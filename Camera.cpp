#include "./Camera.h"
#include <iostream>
#include "globals.h"

void Camera::setPosition() {
    if (focusX == 0 || focusY == 0) {
        x = 0;
        y = 0;
    }
    int half_width =  SCREEN_WIDTH / SCALE / 2;
    int half_height = SCREEN_HEIGHT / SCALE / 2;

    if (*focusX < half_width)
        sourceRect.x = 0;
    else if (*focusX > width - half_width)
        sourceRect.x = width - (half_width* 2);
    else
        sourceRect.x = *focusX - half_width;

    if (*focusY < half_height)
        sourceRect.y = 0;
    else if (*focusY > height - half_height)
        sourceRect.y = height - (half_height * 2);
    else
        sourceRect.y = *focusY - half_height;

    x = sourceRect.x;
    y = sourceRect.y;

    /* THIS IS A COOL 3D ANGLE SHIFT TO PLAY WITH LATER */
    // if (frameCount < 300)
    // {
    //     sourceRect.h += 1;
    //     if(frameCount % 5 == 0)
    //         sourceRect.w -= 1;
    // }
}

void Camera::setFocus(int *fX, int *fY) {
    focusX = fX;
    focusY = fY;
}


void Camera::render() {
    SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
}