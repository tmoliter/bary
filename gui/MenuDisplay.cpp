#include "MenuDisplay.h"

MenuDisplay::MenuDisplay(vector<string> o, Point p, int w, int h, int mC, bool a) : 
    allOptions(o), 
    position(p), 
    maxColumns(mC), 
    currentSelection(0), 
    active(a) 
{
    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    box = nullptr;
    header = nullptr;
    // These need to happen first
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
    delete box;
    delete header;
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
        columns.push_back(new Text(Point(position.x + xPadding + totalArrowPadding + columnOffset, position.y + yPadding), tmp));
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
    renderHeader();
    renderBox();
    for (auto c : columns)
        c->render();
    renderArrow();
    renderPageIndicators();
}

void MenuDisplay::renderHeader() {
    if (header == nullptr)
        return;
    SDL_Rect renderRect = { position.x, position.y - header->sourceRect.h, header->sourceRect.w, header->sourceRect.h };
    SDL_RenderCopy(renderer, header->texture->texture, &header->sourceRect, &renderRect);
}

void MenuDisplay::renderBox() {
    if (box == nullptr)
        return;
    SDL_Rect renderRect = { position.x, position.y, width, height };
    SDL_RenderCopy(renderer, box->texture->texture, &box->sourceRect, &renderRect);
}

void MenuDisplay::renderArrow() {
    int currentColumn = currentSelection % maxColumns;
    int xOffset = ((currentColumn * (charsPerColumn + 2)) + 1) * LETTER_WIDTH;
    int yOffset = ((currentSelection / maxColumns) % maxRows) * LETTER_HEIGHT;
    SDL_Rect arrowRenderRect = { position.x + xPadding + xOffset, position.y + yPadding + yOffset, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_RenderCopy(renderer, font, &rightArrow, &arrowRenderRect);
}

void MenuDisplay::renderPageIndicators() {
    if (frameCount % 30 < 15 || paginatedOptions.size() < 2)
        return;
    int halfwayX = position.x + (width/ 2) - (LETTER_WIDTH / 2);
    if (getCurrentPage() < paginatedOptions.size() - 1) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y + height - LETTER_HEIGHT - (yPadding / 4), LETTER_WIDTH, LETTER_HEIGHT };
        SDL_RenderCopy(renderer, font, &downArrow, &arrowRenderRect);
    }
    if (getCurrentPage() > 0) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y + (yPadding / 4), LETTER_WIDTH, LETTER_HEIGHT };
        SDL_RenderCopy(renderer, font, &upArrow, &arrowRenderRect);
    }
}

void MenuDisplay::setHeight(int newHeight) {
    height = newHeight;

    int defaultPadding = LETTER_HEIGHT;
    yPadding = defaultPadding + ((height % defaultPadding) / 2);
    int useableHeight = height - (yPadding * 2);
    maxRows = useableHeight / LETTER_HEIGHT;
}

void MenuDisplay::setWidth(int newWidth) {
    width = newWidth;

    int defaultPadding = LETTER_WIDTH;
    xPadding = defaultPadding + ((width % defaultPadding) / 2);
    int useableWidth = width - (xPadding * 2);
    int whitespacePerColumn = 2 * LETTER_WIDTH;
    int letterSpacePerColumn = (useableWidth / maxColumns) - whitespacePerColumn;
    charsPerColumn = letterSpacePerColumn / LETTER_WIDTH;
}

void MenuDisplay::addBox(string textureName, SDL_Rect sourcRect) {
    box = new Image(textureName, sourcRect);
}

void MenuDisplay::addHeader(string textureName, SDL_Rect sourcRect) {
    header = new Image(textureName, sourcRect);
}

// GameState should be managed higher up somewhere
void MenuDisplay::setActive(bool a) {
    if (a)
        gameState = GameState::FieldUI;
    else
        gameState = GameState::FieldFree;
    active = a;
}
