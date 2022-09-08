#include "./Phrase.h"

inline constexpr int LETTER_WIDTH = 8;
inline constexpr int LETTER_HEIGHT = 12;
inline constexpr int LETTERS_PER_FONT_ROW = 24;

Phrase::Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, string t) : parent(p), offset(o), start(0) {
    letterLength = pixelWidth / 8;
    letterHeight = pixelHeight / 12;
    int totalLetterLimit = letterLength * letterHeight;
    if (t.length() > totalLetterLimit) {
        text = t.substr(0, totalLetterLimit - 1) + char(127);
    } else {
        text = t;
    }
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
}

void Phrase::progDisplay(int delay) {
    if (start == 0)
        start = frameCount;
    int charsToDisplay = (frameCount - start) / delay;
    cout << charsToDisplay << endl;
    string display;
    if(charsToDisplay < text.length())
        display = text.substr(0, charsToDisplay);
    else
        display = text;
    
    for (int i = 0; i < display.length(); i++) {
        renderLetter(i, display[i]);
    }
}

void Phrase::renderLetter(int position, int asciiValue) {

    int adjustedFontValue = asciiValue - 32;
    int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
    int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
    SDL_Rect sourceRect = { fontX, fontY, LETTER_WIDTH, LETTER_HEIGHT };

    int xPosition = parent.x + offset.x + ((position % letterLength) * LETTER_WIDTH);
    int yPosition = parent.y + offset.y + ((position / letterLength) * LETTER_HEIGHT);
    SDL_Rect renderRect = { xPosition * 4, yPosition * 4, LETTER_WIDTH * 4, LETTER_HEIGHT * 4 };

    SDL_RenderCopy(renderer, Phrase::font, &sourceRect, &renderRect);
}

SDL_Texture *Phrase::font = nullptr;