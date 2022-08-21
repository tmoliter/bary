#include "./Background.h"
#include <iostream>

void Background::setPosition() {
    int half_width =  screen_width / scale / 2;
    int half_height = screen_height / scale / 2;

    if (focusX < half_width)
        sourceRect.x = 0;
    else if (focusX > width - half_width)
        sourceRect.x = width - half_width;
    else
        sourceRect.x = focusX - half_width;

    std::cout << focusY << " : " << half_height << std::endl << std::endl;
    if (focusY < half_height)
        sourceRect.y = 0;
    else if (focusY > height - half_height)
        sourceRect.y = height - half_height;
    else
        sourceRect.y = focusY - half_height;

    *cameraX = sourceRect.x;
    *cameraY = sourceRect.y;
}

void Background::render() {
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
}