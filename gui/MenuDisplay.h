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
    MenuDisplay(vector<string> o, Point p, int h, int w, int mC, bool a = true);
    ~MenuDisplay();

    bool active;
    int currentSelection, charsPerColumn, height, width, maxRows, maxColumns, xPadding, yPadding;

    // optional background

    vector<string> allOptions;
    vector<vector<string>> paginatedOptions;
    vector<Text*> columns;

    Point position;

    SDL_Rect bubbleSourceRect = { 0, 0, 640, 480 };
    SDL_Rect rightArrow = { LETTER_WIDTH * 0, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_Rect upArrow = { LETTER_WIDTH * 2, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };
    SDL_Rect downArrow = { LETTER_WIDTH * 3, LETTER_HEIGHT * 4, LETTER_WIDTH, LETTER_HEIGHT };


    // string helpText
    // TODO: we need to be able to define a constant sized box for ther menu as well as
    // a constant sized box for any help text that we define.
    // IM FACT we should always define a constant size. We should also build our lists left to right
    // and then top to bottom, so we'll almost always need as many Texts as we have possible columns
    // ALSO we should add pagination. This means we might need to expand our font table somehow,
    // probably just put the arrows on the bottom row. Then we can have blinking up and down arrows
    // to show that there are pages


    void buildPages();
    void createLists();
    void clearLists();

    void setHeight(int newHeight);
    void setWidth(int newWidth);

    void moveSelection(Direction direction);
    int getCurrentPage();
    string getCurrentSelection();

    void render();

    void setActive(bool a);

    string getChoice();
};

#endif
