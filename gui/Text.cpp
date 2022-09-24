#include "./Text.h"

using namespace std;

Text::Text(Point p, string t) :
    position(p), text(t) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
}

void Text::render() {
    int line = 0;
    int prevLinesLettercount = 0;
    for (int i = 0; i < text.length();i++) {
        if (int(text[i]) == 96) {
            prevLinesLettercount = i + 1;
            line++;
            continue;
        }
        int adjustedFontValue = int(text[i]) - 32;
        int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
        int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
        SDL_Rect sourceRect = { fontX, fontY, LETTER_WIDTH, LETTER_HEIGHT};

        int xPosition = position.x + ((i - prevLinesLettercount) * LETTER_WIDTH);
        int yPosition = position.y + (line * LETTER_HEIGHT);
        SDL_Rect renderRect = { xPosition, yPosition, LETTER_WIDTH, LETTER_HEIGHT };

        SDL_RenderCopy(renderer, font, &sourceRect, &renderRect);
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