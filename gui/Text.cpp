#include "./Text.h"

using namespace std;

Text::Text(Point p, string t, int lL) :
    position(p), text(t), lineLength(lL) {
    if (!font) {
        resourceDepository::loadTexture("font","fonts/paryfont4rows");
        font = resourceDepository::getTexture("font")->texture;
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
        int fontX = (adjustedFontValue % settings.LETTERS_PER_FONT_ROW) * settings.LETTER_WIDTH;
        int fontY = (adjustedFontValue / settings.LETTERS_PER_FONT_ROW) * settings.LETTER_HEIGHT;
        SDL_Rect sourceRect = { fontX, fontY, settings.LETTER_WIDTH, settings.LETTER_HEIGHT};

        int xPosition = position.x + ((i - totalLettersAfterPrevLine) * settings.LETTER_WIDTH);
        int yPosition = position.y + (line * settings.LETTER_HEIGHT);
        SDL_Rect renderRect = { xPosition, yPosition, settings.LETTER_WIDTH, settings.LETTER_HEIGHT };

        SDL_RenderCopy(renderer, font, &sourceRect, &renderRect);
        currentLinesLettercount++;
    }
}

void Text::setText(string t) {
    text = t;
}

void Text::setLineLengthFromPixelWidth(int pixelWidth) {
    lineLength = pixelWidth / settings.LETTER_WIDTH;
}

void Text::clearText() {
    text.clear();
}


void Text::setPos(Point p) {
    position = p;
}

void Text::resetLineLength() {
    lineLength = (settings.SCREEN_WIDTH / settings.LETTER_WIDTH) - (position.x / settings.LETTER_WIDTH);
}