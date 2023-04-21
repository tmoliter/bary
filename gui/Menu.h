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

struct Menu {
    Menu(vector<string> o, Point p, int mH = -1, int cPc = -1, bool a = true);
    ~Menu();

    bool active;
    int currentSelection, maxRows, charsPerColumn;

    // optional background

    vector<string> options;
    vector<Text*> columns;

    Point position;

    SDL_Rect bubbleSourceRect = { 0, 0, 640, 480 };
    SDL_Rect arrowSourceRect = { LETTER_WIDTH * 23, LETTER_HEIGHT * 3, LETTER_WIDTH, LETTER_HEIGHT };

    int handleInput(KeyPresses keysDown);

    void createLists();
    void clearLists();
    void resetHeight();
    void resetHeight(int newMaxHeight);
    void resetColumnWidth();
    void resetColumnWidth(int newCharsPerColumn);

    void render();

    void setActive(bool a);

    // Should the UIRenderer render the arrow, actually? Or find some other re-usable way so we don't have to re-implement for other pointy things

    string getChoice();
};

#endif
