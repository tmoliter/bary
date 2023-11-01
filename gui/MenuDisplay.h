#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <string>
#include <vector>
#include "Input.h"
#include "resourceDepository.h"
#include "Ray.h"
#include "./Text.h"

using namespace std;

struct MenuDisplay {
    MenuDisplay(vector<string> o, Point p, int w, int h,  int mC, bool a = true);
    ~MenuDisplay();

    bool active;
    int currentSelection, charsPerColumn, height, width, maxRows, maxColumns, xPadding, yPadding;

    vector<string> allOptions;
    vector<vector<string>> paginatedOptions;
    vector<Text*> columns;
    vector<string> flavor;

    Image* header;
    Image* box;

    Point position;

    // SDL_Rect bubbleSourceRect = { 0, 0, 640, 480 };
    SDL_Rect rightArrow = { LETTER_WIDTH * 0, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_Rect upArrow = { LETTER_WIDTH * 2, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_Rect downArrow = { LETTER_WIDTH * 3, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };


    void addFlavor(vector<string> newFlavor);
    void buildPages();
    void createLists();
    void clearLists();

    void processInput(KeyPresses keysDown);
    void moveSelection(Direction direction);
    int getCurrentPage();
    string getCurrentSelection();

    void render();

    void renderBox();
    void renderHeader();
    void renderArrow();
    void renderPageIndicators();

    void setHeight(int newHeight);
    void setWidth(int newWidth);

    void addHeader(string textureName, SDL_Rect sourcRect);
    void addBox(string textureName, SDL_Rect sourcRect);

    void setActive(bool a);

    string getChoice();
};

#endif
