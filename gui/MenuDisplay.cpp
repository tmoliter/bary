#include "MenuDisplay.h"

MenuDisplay::MenuDisplay(vector<string> o, Point p, int h, int w, int cPc, bool a) : allOptions(o), position(p), currentSelection(0), active(a) {
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
    // These need to happen first, and setColumnWidth() has to be first of all
    setColumnWidth(cPc);
    setHeight(h);
    setWidth(w);

    // This might get called again in a "updateOptions" fucntion later
    buildPages();

    // This gets called whenever we change pages as well
    createLists();

    setActive(a);
}

MenuDisplay::~MenuDisplay() {
    clearLists();
}

void MenuDisplay::buildPages() {
    int itemsPerPage = maxColumns * maxRows;
    for (auto it = allOptions.begin(); it != allOptions.end(); it++) {
        int index = it - allOptions.begin();
        if (index % itemsPerPage == 0)
            paginatedOptions.push_back({ *it });
        else
            paginatedOptions[index / itemsPerPage].push_back(*it);
    }
};

void MenuDisplay::createLists() {
    clearLists();
    int i,j;
    vector<string> options = paginatedOptions[getCurrentPage()];

    for (i = 0; i < options.size() && i < maxColumns; i++) {
        string tmp = "";
        int totalArrowPadding = ((i + 1) * 2) * LETTER_WIDTH;
        int columnOffset = i * charsPerColumn * LETTER_WIDTH;
        j = i;
        for (j = i; j < options.size(); j += maxColumns) {
            string option = options[j];
            tmp = tmp + option.substr(0, charsPerColumn) + "`";
        }
        columns.push_back(new Text(Point(position.x + totalArrowPadding + columnOffset, position.y), tmp));
        tmp = "";
    }
}

void MenuDisplay::clearLists() {
    for (auto c : columns)
        delete c;
    columns.clear();
}

void MenuDisplay::moveSelection(Direction direction) {
    int originPage = getCurrentPage();
    switch(direction) {
        case Direction::down:
            if (currentSelection + maxColumns < allOptions.size())
                currentSelection += maxColumns;
            else 
                currentSelection = allOptions.size() - 1;
            break;
        case Direction::up:
            if (currentSelection - maxColumns >= 0)
                currentSelection -= maxColumns;
            break;
        case Direction::left:
            if (currentSelection == 0)
                currentSelection = allOptions.size() - 1;
            else 
                currentSelection--;
            break;
        case Direction::right:
            if (currentSelection == allOptions.size() - 1)
                currentSelection = 0;
            else 
                currentSelection++;
            break;
        case Direction::none:
            return;
    }
    if (originPage != getCurrentPage())
        createLists();
}

string MenuDisplay::getCurrentSelection() {
    return allOptions[currentSelection];
}

int MenuDisplay::getCurrentPage() {
    int itemsPerPage = maxColumns * maxRows;
    return currentSelection / itemsPerPage;
}

void MenuDisplay::render() {
    Point positionWithPadding = Point(position.x - (LETTER_WIDTH / 2), position.y - (LETTER_HEIGHT / 2));
    SDL_Rect bubbleRenderRect = { positionWithPadding.x, positionWithPadding.y, width, height };
    SDL_RenderCopy(renderer, defaultSpeechBubble, &bubbleSourceRect, &bubbleRenderRect);

    int currentColumn = currentSelection % maxColumns;
    int xOffset = ((currentColumn * (charsPerColumn + 2)) + 1) * LETTER_WIDTH;
    int yOffset = ((currentSelection / maxColumns) % maxRows) * LETTER_HEIGHT;
    SDL_Rect arrowRenderRect = { position.x + xOffset, position.y + yOffset, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_RenderCopy(renderer, font, &rightArrow, &arrowRenderRect);

    for (auto c : columns)
        c->render();

    if (frameCount % 30 < 15 || paginatedOptions.size() < 2)
        return;
    int halfwayX = position.x + ((width - position.x) / 2);
    if (getCurrentPage() < paginatedOptions.size() - 1) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y + height - (LETTER_HEIGHT + (LETTER_HEIGHT / 2)), LETTER_WIDTH, LETTER_HEIGHT };
        SDL_RenderCopy(renderer, font, &downArrow, &arrowRenderRect);
    }
    if (getCurrentPage() > 0) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y - (LETTER_HEIGHT / 2), LETTER_WIDTH, LETTER_HEIGHT };
        SDL_RenderCopy(renderer, font, &upArrow, &arrowRenderRect);
    }
}

void MenuDisplay::setColumnWidth() {
   charsPerColumn = -1;
   for (auto o : allOptions) {
        int len = o.length();
        if (len > charsPerColumn)
            charsPerColumn = len;
   }
}

void MenuDisplay::setColumnWidth(int newCharsPerColumn) {
    if (newCharsPerColumn < 1) {
        setColumnWidth();
        return;
    }
    charsPerColumn = newCharsPerColumn;
}

void MenuDisplay::setHeight(int newHeight) {
    int yPadding = LETTER_HEIGHT;
    int useableHeight = newHeight - yPadding;
    height = useableHeight - (useableHeight % LETTER_HEIGHT) + yPadding;
    maxRows = useableHeight / LETTER_HEIGHT;
}

void MenuDisplay::setWidth(int newWidth) {
    int xPadding = LETTER_WIDTH * 2;
    int useableWidth = newWidth - xPadding;
    width = useableWidth - (useableWidth % LETTER_WIDTH) + xPadding;
    maxColumns = useableWidth / ((charsPerColumn + 2) * LETTER_WIDTH);
}

void MenuDisplay::setActive(bool a) {
    if (a)
        gameState = GameState::FieldUI;
    else
        gameState = GameState::FieldFree;
    active = a;
}
