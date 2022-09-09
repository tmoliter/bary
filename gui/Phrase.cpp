#include "./Phrase.h"

using namespace std;

inline constexpr int LETTER_WIDTH = 8;
inline constexpr int LETTER_HEIGHT = 12;
inline constexpr int LETTERS_PER_FONT_ROW = 24;

Phrase::Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, int scale, ScrollType type, string t) : 
    parent(p), 
    offset(o), 
    phraseScale(scale), 
    scrollType(type), 
    progStart(-1),
    advanceStart(-1),
    fullyDisplayed(false) {
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
                    linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
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
    if(!fullyDisplayed)
        return;
    if (hiddenLines.size() < 1) {
        while(!lines.empty()) lines.pop();
        return;
    }
    advanceStart = frameCount;
    fullyDisplayed = false;
};


int Phrase::progDisplay(int delay) {
    if(lines.size() < 0 && hiddenLines.size() < 0)
        return 0;
    
    if (progStart < 0)
        progStart = frameCount;
    int charsToDisplay = (frameCount - progStart) / delay;

    
    int advanceProgress = -1;
    if (advanceStart > -1)
        advanceProgress = (frameCount - advanceStart) / delay;


    queue<string> tmpLines = lines;
    int linesSize = tmpLines.size();
    int total = 0;
    int i, j, occlusion;
    string line;

    // Render letters
    for (i = 0; i < linesSize; i++) {
        line = tmpLines.front();
        if (i == 0)
            occlusion = advanceProgress;
        else
            occlusion = 0;
        for (j = 0; j < line.size(); j++) {
            if (charsToDisplay < 1)
                return 1;
            renderLetter(i, j, line[j], occlusion, advanceProgress);
            charsToDisplay--;
            total++;
        }
        tmpLines.pop();
    }
    fullyDisplayed = true;

    // Begin another scroll cycle immediately for continuous
    if (scrollType == ScrollType::continuous && advanceStart < 0) {
        advance();
    }

    // Show ellipses
    if (
        (scrollType == ScrollType::oneLine || scrollType == ScrollType::allButLast) && 
        frameCount % 60 < 30 &&
        hiddenLines.size() > 0
    )
        renderLetter(i - 1, j, 127, occlusion, advanceProgress);

    // One line's worth of scrolling has completed
    if(advanceProgress >= LETTER_HEIGHT) {
        int lastLineLength;
        switch(scrollType) {
            case (ScrollType::allButLast):
            case (ScrollType::oneLine):
                lastLineLength = lines.front().length();
                lines.pop();
                lines.push(hiddenLines.front());
                hiddenLines.pop();

                advanceStart = -1;
                progStart = frameCount - (total * delay) + lastLineLength;
                break;
            case (ScrollType::continuous):
                if (lines.size() < 1)
                    return 0;
                lastLineLength = lines.front().length();
                advanceStart = frameCount;
                progStart = frameCount - (total * delay) + lastLineLength;
                lines.pop();
                if(hiddenLines.size() < 1)
                    return 1;
                lines.push(hiddenLines.front());
                hiddenLines.pop();
                break;
            default:
                break;
        }
    }
    return 1;
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