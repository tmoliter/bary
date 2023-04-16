#include "./Text.h"

using namespace std;

Text::Text(Point p, string t, int lL) :
    position(p), text(t), lineLength(lL) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    if (lineLength < 0)
        resetLineLength();
}

void Text::render() {
    int line = 0;
    int totalLettersAfterPrevLine = 0;
    int currentLinesLettercount = 0;
    for (int i = 0; i < text.length();i++) {
        if (int(text[i]) == 96) {
            totalLettersAfterPrevLine = i + 1;
            currentLinesLettercount = 0;
            line++;
            continue;
        }
        if (currentLinesLettercount % lineLength == 0 && currentLinesLettercount != 0){
            totalLettersAfterPrevLine = i;
            currentLinesLettercount = 0;
            line++;
        }
        int adjustedFontValue = int(text[i]) - 32;
        int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
        int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
        SDL_Rect sourceRect = { fontX, fontY, LETTER_WIDTH, LETTER_HEIGHT};

        int xPosition = position.x + ((i - totalLettersAfterPrevLine) * LETTER_WIDTH);
        int yPosition = position.y + (line * LETTER_HEIGHT);
        SDL_Rect renderRect = { xPosition, yPosition, LETTER_WIDTH, LETTER_HEIGHT };

        SDL_RenderCopy(renderer, font, &sourceRect, &renderRect);
        currentLinesLettercount++;
    }
}

void Text::setText(string t) {
    text = t;
}

void Text::clearText() {
    text.clear();
}


void Text::setPos(Point p) {
    position = p;
}

void Text::resetLineLength() {
    lineLength = (SCREEN_WIDTH / LETTER_WIDTH) - (position.x / LETTER_WIDTH);
}