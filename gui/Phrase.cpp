#include "./Phrase.h"

inline constexpr int LETTER_WIDTH = 8;
inline constexpr int LETTER_HEIGHT = 12;
inline constexpr int LETTERS_PER_FONT_ROW = 24;

Phrase::Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, int pS, string t) : parent(p), offset(o), phraseScale(pS), start(0) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    letterLength = pixelWidth / 8;
    letterHeight = pixelHeight / 12;
    int totalLetterLimit = letterLength * letterHeight;
    if (t.length() > totalLetterLimit) {
        text = t.substr(0, totalLetterLimit - 1) + char(127);
    } else {
        text = t;
    }
    if (text.length() <= letterLength)
        lines.push(text);
    else {
        int i = 0;
        int lastSpace = 0;
        int lineFirstCharIndex = 0;
        while(i < text.length()) {
            cout << "char: " << text[i] << endl;
            if (i == text.length() - 1) {
                lines.push(text.substr(lineFirstCharIndex, i));
                cout << "A: " << text.substr(lineFirstCharIndex, i) << endl;
                break;
            }
            if (int(text[i]) == 32) {
                cout << "B" << endl;
                lastSpace = i;
            }
            if (i > 0 && i - lineFirstCharIndex == letterLength) {
                cout << "LINES SIZE: " << lines.size() << endl;
                if (lines.size() >= letterHeight - 1) {
                    lines.push(text.substr(lineFirstCharIndex, letterLength - 1) + char(127));
                    cout << "E: " << text.substr(lineFirstCharIndex, letterLength - 1) + "..." << endl;
                    break;
                }
                if (lastSpace == i) {
                    lines.push(text.substr(lineFirstCharIndex, letterLength));
                    cout << "C: " << text.substr(lineFirstCharIndex, letterLength) << endl;
                    i++;
                    lineFirstCharIndex = i;
                    continue;
                }
                if (lastSpace <= lineFirstCharIndex) {
                    lines.push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    cout << "D: " << text.substr(lineFirstCharIndex, letterLength - 1) + char(45) << endl;
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                if (i - lastSpace > letterLength / 2) {
                    lines.push(text.substr(lineFirstCharIndex, letterLength - 1) + char(45));
                    cout << "E: " << text.substr(lineFirstCharIndex, letterLength - 1) + char(45) << endl;
                    i--;
                    lineFirstCharIndex = i;
                    continue;
                }
                lines.push(text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex));
                cout << "F: " << text.substr(lineFirstCharIndex, lastSpace - lineFirstCharIndex) << endl;
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

void Phrase::progDisplay(int delay) {
    if (start == 0)
        start = frameCount;
    int charsToDisplay = (frameCount - start) / delay;
    string display;
    if(charsToDisplay > text.length())
        display = text.substr(0, charsToDisplay);
    else
        display = text;

    int linesSize = lines.size();
    int total = 0;
    queue<string> tmpLines = lines;
    for (int i = 0; i < linesSize; i++) {
        string line = tmpLines.front();
        for (int j = 0; j < line.size(); j++) {
            if (charsToDisplay < 1)
                return;
            renderLetter(i, j, line[j]);
            charsToDisplay--;
        }
        tmpLines.pop();
    }
}

void Phrase::renderLetter(int lineNumber, int position, int asciiValue) {

    int adjustedFontValue = asciiValue - 32;
    int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
    int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
    SDL_Rect sourceRect = { fontX, fontY, LETTER_WIDTH, LETTER_HEIGHT };

    int xPosition = parent.x + offset.x + (position * LETTER_WIDTH);
    int yPosition = parent.y + offset.y + (lineNumber * LETTER_HEIGHT);
    SDL_Rect renderRect = { xPosition * phraseScale, yPosition * phraseScale, LETTER_WIDTH * phraseScale, LETTER_HEIGHT * phraseScale };

    SDL_RenderCopy(renderer, Phrase::font, &sourceRect, &renderRect);
}

SDL_Texture *Phrase::font = nullptr;