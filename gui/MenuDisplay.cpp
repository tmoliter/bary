#include "MenuDisplay.h"

MenuDisplay::MenuDisplay(vector<string> o, Point p, int mH, int cPc, bool a) : options(o), position(p), currentSelection(0), active(a) {
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    if (!defaultSpeechBubble) {
        SDL_Surface* temp = IMG_Load("assets/speechBubbles/defaultSpeechBubble.png");
        defaultSpeechBubble = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    resetHeight(mH);
    resetColumnWidth(cPc);
    createLists();
    setActive(a);
}

MenuDisplay::~MenuDisplay() {
    clearLists();
}

void MenuDisplay::createLists() {
    clearLists();
    int i = 0;
    string tmp = "";
    while (i < options.size()) {
        tmp = tmp + options[i++];
        if (i % maxRows == 0 || i == options.size()) {
            int columnNumber = (i - 1) / maxRows;
            int totalArrowPadding = (columnNumber + 2) * LETTER_WIDTH;
            int columnOffset = columnNumber * charsPerColumn * LETTER_WIDTH;
            columns.push_back(new Text(Point(position.x + totalArrowPadding + columnOffset, position.y), tmp));
            tmp = "";
            continue;
        }
        tmp = tmp + "`";
    }
}

void MenuDisplay::clearLists() {
    for (auto c : columns)
        delete c;
    columns.clear();
}

void MenuDisplay::render() {
    Point positionWithPadding = Point(position.x - LETTER_HEIGHT, position.y - LETTER_HEIGHT);
    int xPadding = LETTER_WIDTH * 2;
    int yPadding = LETTER_HEIGHT;
    SDL_Rect bubbleRenderRect = { positionWithPadding.x, positionWithPadding.y, int(columns.size()) * ((2 + charsPerColumn) * LETTER_WIDTH) + xPadding, ((maxRows + 1) * LETTER_HEIGHT) + yPadding };
    SDL_RenderCopy(renderer, defaultSpeechBubble, &bubbleSourceRect, &bubbleRenderRect);
    int currentColumn = currentSelection % maxRows;
    int xOffset = ((currentColumn * (charsPerColumn + 2)) + 1) * LETTER_WIDTH;
    int yOffset = (currentSelection % maxRows) * LETTER_HEIGHT;
    SDL_Rect arrowRenderRect = { position.x + xOffset, position.y + yOffset, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_RenderCopy(renderer, font, &arrowSourceRect, &arrowRenderRect);
    for (auto c : columns)
        c->render();
}

void MenuDisplay::resetColumnWidth() {
   charsPerColumn = -1;
   for (auto o : options) {
        int len = o.length();
        if (len > charsPerColumn)
            charsPerColumn = len;
   }
}

void MenuDisplay::resetColumnWidth(int newCharsPerColumn) {
    resetColumnWidth();
    if (newCharsPerColumn > charsPerColumn)
        charsPerColumn = newCharsPerColumn;
}

void MenuDisplay::resetHeight() {
    maxRows = (SCREEN_HEIGHT - position.y) / LETTER_HEIGHT;
}

void MenuDisplay::resetHeight(int newMaxHeight) {
    if (newMaxHeight < LETTER_HEIGHT) {
        resetHeight();
        return;
    }
    maxRows = newMaxHeight / LETTER_HEIGHT;
}

void MenuDisplay::setActive(bool a) {
    if (a)
        gameState = GameState::FieldUI;
    else
        gameState = GameState::FieldFree;
    active = a;
}
