#include "./Phrase.h"

using namespace std;

inline constexpr int LETTER_WIDTH = 8;
inline constexpr int LETTER_HEIGHT = 12;
inline constexpr int LETTERS_PER_FONT_ROW = 24;

Phrase::Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, int pS, string t) : parent(p), offset(o), phraseScale(pS), progStart(0), advanceStart(0) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    letterLength = pixelWidth / 8;
    letterHeight = pixelHeight / 12;
    text = t;

    // Very happy case: all text fits on one line
    if (text.length() <= letterLength)
        lines.push(text);
    else {
        int i = 0;
        int lastSpace = 0;
        int lineFirstCharIndex = 0;
        bool bonusTime = false;
        queue<string> *linesRef = &lines;
        while(i < text.length()) {
            cout << "char: " << text[i] << "(" << int(text[i]) << ")" << endl;
            // Made it to end and all of current line fits on a line
            if (i == text.length() - 1) {
                linesRef->push(text.substr(lineFirstCharIndex, i));
                cout << "A: " << text.substr(lineFirstCharIndex, i) << endl;
                break;
            }
            // Mark space, unless this is the last character of the last line
            if (int(text[i]) == 32 && 
                !(!bonusTime && 
                    linesRef->size() >= letterHeight - 1 && 
                    i - lineFirstCharIndex == letterLength
                )) {
                cout << "B" << endl;
                lastSpace = i;
            }
            // Ran out of room for line
            if (i > 0 && i - lineFirstCharIndex == letterLength) {
                // Ran out of lines that will fit
                if (!bonusTime && linesRef->size() >= letterHeight - 1) {
                    linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex) + char(127));
                    cout << "C: " << text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex)  + "..." << endl;
                    bonusTime = true;
                    linesRef = &hiddenLines;
                    i = lastSpace + 1;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Word ends cleanly at end of line
                if (lastSpace == i) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength));
                    cout << "D: " << text.substr(lineFirstCharIndex, letterLength) << endl;
                    i++;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word takes up more than a whole line
                if (lastSpace <= lineFirstCharIndex) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    cout << "E: " << text.substr(lineFirstCharIndex, letterLength - 1) + char(45) << endl;
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word would get pushed onto next line, but it is large enough to look weird
                if (i - lastSpace > letterLength / 2) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    cout << "F: " << text.substr(lineFirstCharIndex, letterLength - 1) + char(45) << endl;
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                // We're in the middle of a word and it's small enough to push to the next line
                linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
                cout << "G: " << text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex) << endl;
                i = lastSpace + 1;
                lineFirstCharIndex = i;
                continue;
            }
            cout << "++" << endl;
            i++;
        }
        cout << endl << endl;
    }
}

void Phrase::advance() {
    if (hiddenLines.size() < 1) {
        while(!lines.empty()) lines.pop();
        return;
    }
    advanceStart = frameCount;
};


void Phrase::progDisplay(int delay) {
    if (progStart == 0)
        progStart = frameCount;

    int charsToDisplay = (frameCount - progStart) / delay;
    
    int advanceProgress = 0;
    if (advanceStart > 0)
        advanceProgress = (frameCount - advanceStart) / delay;

    queue<string> tmpLines = lines;
    int linesSize = tmpLines.size();
    int total = 0;
    for (int i = 0; i < linesSize; i++) {
        string line = tmpLines.front();
        int occlusion = 0;
        if (i == 0)
            occlusion = advanceProgress;
        for (int j = 0; j < line.size(); j++) {
            if (charsToDisplay < 1)
                return;
            renderLetter(i, j, line[j], occlusion, advanceProgress);
            charsToDisplay--;
            total++;
        }
        tmpLines.pop();
    }

    if(advanceProgress >= LETTER_HEIGHT) {
        int lastLineLength = lines.front().length();
        lines.pop();
        lines.push(hiddenLines.front());
        hiddenLines.pop();

        advanceStart = 0;
        progStart = frameCount - (total * delay) + lastLineLength;
    }
}

void Phrase::renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise) {

    int adjustedFontValue = asciiValue - 32;
    int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
    int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
    SDL_Rect sourceRect = { fontX, fontY + occlusion, LETTER_WIDTH, LETTER_HEIGHT - occlusion};

    int xPosition = parent.x + offset.x + (position * LETTER_WIDTH * phraseScale);
    int yPosition = parent.y + offset.y + (lineNumber * LETTER_HEIGHT * phraseScale) - (raise * phraseScale);
    SDL_Rect renderRect = { xPosition, yPosition, LETTER_WIDTH * phraseScale, (LETTER_HEIGHT - occlusion) * phraseScale };

    SDL_RenderCopy(renderer, Phrase::font, &sourceRect, &renderRect);
}

SDL_Texture *Phrase::font = nullptr;