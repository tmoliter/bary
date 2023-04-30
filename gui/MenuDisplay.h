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
    MenuDisplay(vector<string> o, Point p, int mH = -1, int cPc = -1, bool a = true);
    ~MenuDisplay();

    bool active;
    int currentSelection, maxRows, charsPerColumn;

    // optional background

    vector<string> options;
    vector<Text*> columns;

    Point position;

    SDL_Rect bubbleSourceRect = { 0, 0, 640, 480 };
    SDL_Rect arrowSourceRect = { LETTER_WIDTH * 23, LETTER_HEIGHT * 3, LETTER_WIDTH, LETTER_HEIGHT };

    // string helpText
    // we need to be able to define a constant sized box for the menu as well as
    // a constant sized box for any help text that we define.

    void createLists();
    void clearLists();
    void resetHeight();
    void resetHeight(int newMaxHeight);
    void resetColumnWidth();
    void resetColumnWidth(int newCharsPerColumn);

    void render();

    void setActive(bool a);

    string getChoice();
};

#endif
