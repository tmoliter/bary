#include "./Background.h"
#include <iostream>

void Background::setPosition() {
    int half_width =  screen_width / scale / 2;
    int half_height = screen_height / scale / 2;

    if (focusX < half_width)
        sourceRect.x = 0;
    else if (focusX > width - half_width)
        sourceRect.x = width - (half_width* 2);
    else
        sourceRect.x = focusX - half_width;

    if (focusY < half_height)
        sourceRect.y = 0;
    else if (focusY > height - half_height)
        sourceRect.y = height - (half_height * 2);
    else
        sourceRect.y = focusY - half_height;

    *cameraX = sourceRect.x;
    *cameraY = sourceRect.y;
}

void Background::render() {
    SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
}