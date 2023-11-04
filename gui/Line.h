#ifndef LINE_H
#define LINE_H
#include <vector>
#include "Camera.h"

using namespace std;

enum class LineType {
    interactable,
    trigger,
    obstruction,
    editing,
    highlight,
    line
};

struct Line {
    Line(int &pX, int &pY, Ray *r, LineType t);

    Ray *ray;
    LineType type;
    int &parentX, &parentY;
    void render();
};

#endif