#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <string>
#include <vector>
#include "resourceDepository.h"
#include "./Text.h"

using namespace std;

struct Option {
    Option() : selectionText(""), flavorText(""), value("") {};
    Option(string sT) : selectionText(sT), value(sT), flavorText("") {};
    Option(string sT, string fT, string value) : selectionText(sT), flavorText(fT), value(value) {};

    string selectionText;
    string flavorText;
    string value;
};

struct MenuDisplay {
    MenuDisplay(vector<Option> o, Point p, Point size, int mC, bool a = true);
    ~MenuDisplay();

    bool active;
    int currentSelection, charsPerColumn, height, width, maxRows, maxColumns, xPadding, yPadding;

    vector<Option> allOptions;
    vector<vector<Option>> paginatedOptions;
    vector<Text*> columns;

    Image* header;
    Image* box;
    Image* flavorBox;

    Point position;
    Text flavorText;

    // SDL_Rect bubbleSourceRect = { 0, 0, 640, 480 };
    SDL_Rect rightArrow = { settings.LETTER_WIDTH * 0, settings.LETTER_HEIGHT * 4, settings.LETTER_WIDTH, settings.LETTER_HEIGHT };
    SDL_Rect upArrow = { settings.LETTER_WIDTH * 2, settings.LETTER_HEIGHT * 4, settings.LETTER_WIDTH, settings.LETTER_HEIGHT };
    SDL_Rect downArrow = { settings.LETTER_WIDTH * 3, settings.LETTER_HEIGHT * 4, settings.LETTER_WIDTH, settings.LETTER_HEIGHT };


    // void addFlavor(vector<string> newFlavor);
    void buildPages();
    void createLists();
    void clearLists();

    bool processInput(KeyPresses keysDown, string& selection);
    void moveSelection(Direction direction);
    int getCurrentPage();
    Option getCurrentSelection();

    void render();

    void renderBox();
    void renderHeader();
    void renderFlavorBox();
    void renderArrow();
    void renderPageIndicators();

    void setHeight(int newHeight);
    void setWidth(int newWidth);

    void addHeader(string textureName, SDL_Rect sourcRect);
    void addFlavorBox(string textureName, SDL_Rect sourcRect);
    void addBox(string textureName, SDL_Rect sourcRect);

    void setActive(bool a);
};

#endif
