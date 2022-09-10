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
    letterLength = (pixelWidth / scale) / 8;
    letterHeight = (pixelHeight / scale) / 12;
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
            // Made it to end and all of current line fits on a line
            if (i == text.length() - 1) {
                linesRef->push(text.substr(lineFirstCharIndex, i));
                break;
            }
            // Mark space, unless this is the last character of the last line
            if (int(text[i]) == 32 && 
                !(!bonusTime && 
                    linesRef->size() >= letterHeight - 1 && 
                    i - lineFirstCharIndex == letterLength
                )) {
                lastSpace = i;
            }
            // Ran out of room for line
            if (i > 0 && i - lineFirstCharIndex == letterLength) {
                // Ran out of lines that will fit
                if (!bonusTime && linesRef->size() >= letterHeight - 1) {
                    linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
                    bonusTime = true;
                    linesRef = &hiddenLines;
                    i = lastSpace + 1;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Word ends cleanly at end of line
                if (lastSpace == i) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength));
                    i++;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word takes up more than a whole line
                if (lastSpace <= lineFirstCharIndex) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word would get pushed onto next line, but it is large enough to look weird
                if (i - lastSpace > letterLength / 2) {
                    linesRef->push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                // We're in the middle of a word and it's small enough to push to the next line
                linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
                i = lastSpace + 1;
                lineFirstCharIndex = i;
                continue;
            }
            i++;
        }
    }
    totalLines = lines.size() + hiddenLines.size();
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

/* TODO: This breaks if the pixelWidth value is above about 220 * scale. Need to fix */
int Phrase::progDisplay(int delay) {
    if(lines.size() < 1 && hiddenLines.size() < 1)
        return 0;
    
    if (progStart < 0)
        progStart = frameCount;
    int charsToDisplay = (frameCount - progStart) / delay;

    
    int advanceProgress = -1;
    if (advanceStart > -1)
        advanceProgress = (frameCount - advanceStart) / (delay * 2);

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

    // One line's worth of scrolling has completed
    if(advanceProgress >= LETTER_HEIGHT) {
        int lastLineLength;
        lastLineLength = lines.front().length();
        lines.pop();
        progStart = frameCount - (total * delay) + (lastLineLength * delay);
        if(hiddenLines.size() > 0) {
            lines.push(hiddenLines.front());
            hiddenLines.pop();
        }
        switch(scrollType) {
            case (ScrollType::allButLast):
                if ((totalLines - lines.size() - hiddenLines.size()) % (letterHeight - 1) == 0)
                    advanceStart = -1;
                else 
                    advanceStart = frameCount + ((advanceProgress - LETTER_HEIGHT) * delay);
                break;
            case (ScrollType::oneLine):
                advanceStart = -1;
                break;
            case (ScrollType::continuous):
                advanceStart = frameCount + ((advanceProgress - LETTER_HEIGHT) * delay);
                return 1;
            default:
                break;
        }
    }

    // Show ellipses
    if (
        (scrollType == ScrollType::oneLine || scrollType == ScrollType::allButLast) && 
        frameCount % 60 < 30 &&
        hiddenLines.size() > 0
    )
        renderLetter(i - 1, j, 127, occlusion, advanceProgress);

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