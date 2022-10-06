#include "Line.h"

Line::Line(int &pX, int &pY, Ray *r, LineType t) : parentX(pX), parentY(pY), ray(r), type(t) {};

void Line::render() {
    switch(type) {
        case LineType::obstruction:
            SDL_SetRenderDrawColor(renderer,255,0,0,255);
            break;
        case LineType::interactable:
            SDL_SetRenderDrawColor(renderer,85,150,255,255);
            break;
        case LineType::trigger:
            SDL_SetRenderDrawColor(renderer,255,255,0,255);
            break;
        case LineType::editing:
            SDL_SetRenderDrawColor(renderer, 100,255,255,255);
            break;
        case LineType::highlight:
            SDL_SetRenderDrawColor(renderer, 180,180,180,180);
            break;
        case LineType::line:
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            break;
    }
    Ray adjusted = Camera::worldToScreen(Ray(Point(parentX + ray->a.x, parentY + ray->a.y), Point(parentX + ray->b.x, parentY + ray->b.y)));
    SDL_RenderDrawLine(renderer, adjusted.a.x, adjusted.a.y, adjusted.b.x, adjusted.b.y);
}
