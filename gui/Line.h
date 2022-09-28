#ifndef LINE_H
#define LINE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "globals.h"
#include "Ray.h"
#include "Camera.h"

using namespace std;

enum LineType {
    interactable,
    obstruction,
    line
};

struct Line {
    Line(int &pX, int &pY, Ray *r, LineType t);

    Ray *ray;
    LineType type;
    int &parentX, &parentY;
    void render();

    inline static vector<Line*> lines;

    static void removeLine(Line *);

    static void renderLines();
    static void renderLines(LineType type);
};

#endif