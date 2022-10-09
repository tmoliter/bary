#include "./Phrase.h"

using namespace std;

Phrase::Phrase(Point p, Point pixelSize, ScrollType type, string t, Point gL, int pS, int d) :
    autoDestroy(true),
    phraseScale(pS),
    scrollType(type),
    delay(d),
    text(t),
    leftPad(0),
    topPad(0),
    gridLimits(gL) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    box = SDL_Rect { p.x, p.y, pixelSize.x, pixelSize.y };
    reset();
}

void Phrase::reset() {
    queue<string>().swap(lines);
    queue<string>().swap(hiddenLines);

    utils::limit(gridLimits.x, 1, (box.w / (phraseScale * LETTER_WIDTH)) - 2);
    utils::limit(gridLimits.y, 1, (box.h / (phraseScale * LETTER_HEIGHT)) - 1);

    // Very happy case: all text fits on one line
    if (text.length() <= gridLimits.x)
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
                    linesRef->size() >= gridLimits.y - 1 && 
                    i - lineFirstCharIndex == gridLimits.x
                )) {
                lastSpace = i;
            }
            // Manual new line with ` char
            if (int(text[i]) == 96) {
                linesRef->push(text.substr(lineFirstCharIndex, i - lineFirstCharIndex));
                if (!bonusTime && linesRef->size() > gridLimits.y) {
                    bonusTime = true;
                    linesRef = &hiddenLines;
                }
                i++;
                lastSpace = i;
                lineFirstCharIndex = i;
                continue;
            }
            // Ran out of room for line
            if (i > 0 && i - lineFirstCharIndex == gridLimits.x) {
                // Ran out of lines that will fit
                if (!bonusTime && linesRef->size() >= gridLimits.y - 1) {
                    linesRef->push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
                    bonusTime = true;
                    linesRef = &hiddenLines;
                    i = lastSpace + 1;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Word ends cleanly at end of line
                if (lastSpace == i) {
                    linesRef->push(text.substr(lineFirstCharIndex, gridLimits.x));
                    i++;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word takes up more than a whole line
                if (lastSpace <= lineFirstCharIndex) {
                    linesRef->push(text.substr(lineFirstCharIndex, gridLimits.x - 1) + char(45));
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                // Current word would get pushed onto next line, but it is large enough to look weird
                if (i - lastSpace > gridLimits.x / 2) {
                    linesRef->push(text.substr(lineFirstCharIndex, gridLimits.x - 1) + char(45));
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

    leftPad = (box.w - (phraseScale * gridLimits.x * LETTER_WIDTH)) / 2;
    topPad = (box.h - (phraseScale * lines.size() * LETTER_HEIGHT)) / 2;

    progStart = -1;
    advanceStart = -1;
    fullyDisplayed = false;
    complete = false;
}

void Phrase::advance() {
    if(!fullyDisplayed || advanceStart > 0)
        return;
    if (hiddenLines.size() < 1) {
        while(!lines.empty()) lines.pop();
        return;
    }
    advanceStart = frameCount;
    fullyDisplayed = false;
};

bool Phrase::isComplete() {
    return complete;
}

int Phrase::progDisplay() {
    if(complete)
        return 0;
    SDL_SetRenderDrawColor(renderer, 100,100,255,255);
    SDL_RenderFillRect(renderer, &box);
    queue<string> tmpLines = lines;
    int linesSize = tmpLines.size();
    int total = 0;
    int i, j, occlusion;
    string line;

    if (scrollType == ScrollType::preview) {
        // This shits fucked up, debug me
        for (i = 0; i < linesSize; i++) {
            line = tmpLines.front();
            for (j = 0; j < line.size(); j++) {
                renderLetter(i, j, line[j], 0, 0);
            }
            tmpLines.pop();
        }
        if (hiddenLines.size() > 0)
            renderLetter(i - 1, j, 127, 0, 0);
        return 0;
    }

    if (progStart < 0)
        progStart = frameCount;
    int charsToDisplay = (frameCount - progStart) / delay;

    
    int advanceProgress = -1;
    if (advanceStart > -1)
        advanceProgress = (frameCount - advanceStart) / (delay * 2);

    // Render letters
    for (i = 0; i < linesSize; i++) {
        line = tmpLines.front();
        if (i == 0)
            occlusion = advanceProgress;
        else
            occlusion = 0;
        for (j = 0; j < line.size(); j++) {
            if (charsToDisplay < 1){
                if(advanceProgress >= LETTER_HEIGHT)
                    advanceStart++;
                return 1;
            }
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
                if ((totalLines - lines.size() - hiddenLines.size()) % (gridLimits.y - 1) == 0)
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

    // End case
    if((lines.size() < 1 && hiddenLines.size() < 1))
        complete = true;
    
    return 1;
}

void Phrase::renderLetter(int lineNumber, int charPosition, int asciiValue, int occlusion, int raise) {

    int adjustedFontValue = asciiValue - 32;
    int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
    int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
    SDL_Rect sourceRect = { fontX, fontY + occlusion, LETTER_WIDTH, LETTER_HEIGHT - occlusion};

    int xPosition = box.x + leftPad + (charPosition * LETTER_WIDTH * phraseScale);
    int yPosition = box.y + topPad + (lineNumber * LETTER_HEIGHT * phraseScale) - (raise * phraseScale);
    SDL_Rect renderRect = { xPosition, yPosition + occlusion, LETTER_WIDTH * phraseScale, (LETTER_HEIGHT - occlusion) * phraseScale };

    SDL_RenderCopy(renderer, font, &sourceRect, &renderRect);
}

SDL_Rect& Phrase::getBox() {
    return box;
}

void Phrase::setGridLimits(DirectionMap dM) {
    if (dM.up && gridLimits.y <= (box.h / (phraseScale * LETTER_HEIGHT)) - 1) {
        gridLimits.y++;
        reset();
    }
    if (dM.down && gridLimits.y > 1) {
        gridLimits.y--;
        reset();
    }
    if (dM.left && gridLimits.x > 10) {
        gridLimits.x--;
        reset();
    }
    if (dM.right && gridLimits.x <= (box.w / (phraseScale * LETTER_WIDTH)) - 2) {
        gridLimits.x++;
        reset();
    }
}