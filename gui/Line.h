#ifndef LINE_H
#define LINE_H
#include <vector>
#include "Camera.h"

using namespace std;

// In order of painting
enum class LineType {
    obstruction,
    interactable,
    trigger,
    line,
    highlight,
    editing
};

struct Line {
    Line(int &pX, int &pY, Ray *r, LineType t);

    Ray *ray;
    LineType type;
    int &parentX, &parentY;
    void render();
};

#endif