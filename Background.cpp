#include "./Background.h"
#include <iostream>

void Background::render(int &centerX, int &centerY) {
    sourceRect.x = centerX - (screen_width / 2);
    std::cout << sourceRect.x << std::endl;
    std::cout << sourceRect.y << std::endl << std::endl;
    sourceRect.y = centerY - (screen_height / 2);
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
}