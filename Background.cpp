#include "./Background.h"

void Background::render(int &centerX, int &centerY) {
    sourceRect.x = centerX - (screen_width / 2);
    sourceRect.y = centerY - (screen_height / 2);
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
}