#include "MenuDisplay.h"

MenuDisplay::MenuDisplay(vector<Option> o, Point p, Point size, int mC) :
    allOptions(o),
    position(p),
    maxColumns(mC),
    currentSelection(0)
{
    box = nullptr;
    header = nullptr;
    flavorBox = nullptr;

    // This is a default font, but we could allow this to be customized
    font = resourceDepository::getTexture("defaultFont");

    // These need to happen first
    setHeight(size.y);
    setWidth(size.x);

    // This might get called again in a "updateOptions" function later
    buildPages();

    // This gets called whenever we change pages as well
    createLists();

    flavorText.setPos(Point(position.x + xPadding, position.y + height + yPadding));
    flavorText.setText(allOptions[0].flavorText);
    flavorText.setLineLengthFromPixelWidth(width - (xPadding * 2));
}

MenuDisplay::~MenuDisplay() {
    clearLists();
    delete box;
    delete header;
    delete flavorBox;
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
    vector<Option> options = paginatedOptions[getCurrentPage()];

    for (i = 0; i < options.size() && i < maxColumns; i++) {
        string tmp = "";
        int totalArrowPadding = ((i + 1) * 2) * settings.LETTER_WIDTH * settings.FONT_SCALE;
        int columnOffset = i * charsPerColumn * settings.LETTER_WIDTH * settings.FONT_SCALE;
        j = i;
        for (j = i; j < options.size(); j += maxColumns) {
            string option = options[j].selectionText;
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

bool MenuDisplay::processInput(KeyPresses keysDown, string& selection) {
    if (keysDown.cancel) {
        selection = "";
        return true;
    }
    if (keysDown.ok)
        return true;
    Direction d = Direction::none;
    if(keysDown.nav_up)
        d = Direction::up;
    if(keysDown.nav_down)
        d = Direction::down;
    if(keysDown.nav_left)
        d = Direction::left;
    if(keysDown.nav_right)
        d = Direction::right;
    moveSelection(d);
    if (d != Direction::none)
        selection = getCurrentSelection().value;
    return false;
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
    flavorText.setText(getCurrentSelection().flavorText);
    if (originPage != getCurrentPage())
        createLists();
}

Option MenuDisplay::getCurrentSelection() {
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
    renderFlavorBox();
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

void MenuDisplay::renderFlavorBox() {
    if (flavorBox == nullptr || box == nullptr || flavorText.text == "")
        return;
    SDL_Rect renderRect = { position.x, position.y + height, width, height };
    SDL_RenderCopy(renderer, flavorBox->texture->texture, &flavorBox->sourceRect, &renderRect);
    flavorText.render();
}

void MenuDisplay::renderArrow() {
    int currentColumn = currentSelection % maxColumns;
    int xOffset = ((currentColumn * (charsPerColumn + 2)) + 1) * settings.LETTER_WIDTH * settings.FONT_SCALE;
    int yOffset = ((currentSelection / maxColumns) % maxRows) * settings.LETTER_HEIGHT * settings.FONT_SCALE;
    SDL_Rect arrowRenderRect = { position.x + xPadding + xOffset, position.y + yPadding + yOffset, settings.LETTER_WIDTH * settings.FONT_SCALE, settings.LETTER_HEIGHT * settings.FONT_SCALE };
    SDL_RenderCopy(renderer, font->texture, &rightArrow, &arrowRenderRect);
}

void MenuDisplay::renderPageIndicators() {
    if (frameCount % 30 < 15 || paginatedOptions.size() < 2)
        return;
    int glyphWidth = settings.LETTER_WIDTH * settings.FONT_SCALE;
    int glyphHeight = settings.LETTER_HEIGHT * settings.FONT_SCALE;
    int halfwayX = position.x + (width/ 2) - (glyphWidth / 2);
    if (getCurrentPage() < paginatedOptions.size() - 1) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y + height - glyphHeight - (yPadding / 4), glyphWidth, glyphHeight };
        SDL_RenderCopy(renderer, font->texture, &downArrow, &arrowRenderRect);
    }
    if (getCurrentPage() > 0) {
        SDL_Rect arrowRenderRect = { halfwayX, position.y + (yPadding / 4), glyphWidth, glyphHeight };
        SDL_RenderCopy(renderer, font->texture, &upArrow, &arrowRenderRect);
    }
}

void MenuDisplay::setHeight(int newHeight) {
    height = newHeight;

    int glyphHeight = settings.LETTER_HEIGHT * settings.FONT_SCALE;
    yPadding = glyphHeight + ((height % glyphHeight) / 2);
    int useableHeight = height - (yPadding * 2);
    maxRows = useableHeight / glyphHeight;
}

void MenuDisplay::setWidth(int newWidth) {
    width = newWidth;

    int glyphWidth = settings.LETTER_WIDTH * settings.FONT_SCALE;
    xPadding = glyphWidth + ((width % glyphWidth) / 2);
    int useableWidth = width - (xPadding * 2);
    int whitespacePerColumn = 2 * glyphWidth;
    int letterSpacePerColumn = (useableWidth / maxColumns) - whitespacePerColumn;
    charsPerColumn = letterSpacePerColumn / glyphWidth;
}

void MenuDisplay::addBox(string textureName, SDL_Rect sourcRect) {
    if (box) {
        cout << "Menu already has a box! \n";
        throw exception();
    }
    box = new Image(textureName, sourcRect);
}

void MenuDisplay::addHeader(string textureName, SDL_Rect sourcRect) {
    if (header) {
        cout << "Menu already has a header! \n";
        throw exception();
    }
    header = new Image(textureName, sourcRect);
}

void MenuDisplay::addFlavorBox(string textureName, SDL_Rect sourcRect) {
    if (flavorBox) {
        cout << "Menu already has a flavorBox! \n";
        throw exception();
    }
    flavorBox = new Image(textureName, sourcRect);
}
