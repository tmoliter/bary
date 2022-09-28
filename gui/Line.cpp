#include "Line.h"

Line::Line(int &pX, int &pY, Ray *r, LineType t) : parentX(pX), parentY(pY), ray(r), type(t) {
    Line::lines.push_back(this);
};

void Line::render() {
    switch(type) {
        case obstruction:
            SDL_SetRenderDrawColor(renderer, 255,0,0,255);
            break;
        case interactable:
            SDL_SetRenderDrawColor(renderer, 0,255,0,255);
            break;
        case line:
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            break;
    }
    Ray adjusted = Camera::worldToScreen(Ray(Point(parentX + ray->a.x, parentY + ray->a.y), Point(parentX + ray->b.x, parentY + ray->b.y)));
    SDL_RenderDrawLine(renderer, adjusted.a.x, adjusted.a.y, adjusted.b.x, adjusted.b.y);
}

void Line::removeLine(Line *l) {
    lines.erase(remove(lines.begin(), lines.end(), l), lines.end());
}