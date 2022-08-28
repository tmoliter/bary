#include <vector>
#include <iostream>
#include "Sprite.h"

using namespace std;

Sprite::Sprite (int &x, int &y, int tI, const char *p) : x(x), y(y), path(p), active(false) {
    id = currentID++;
    Sprite::sprites[id] = this;
    renderer = Camera::c->renderer;
    cameraX = &Camera::c->x;
    cameraY = &Camera::c->y;
    SDL_Surface* temp = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    renderRect = {
        -1000,
        -1000,
        width * SCALE, 
        height * SCALE 
    };
    sourceRect = { 0, 0, width, height };
    active = true;
}

Sprite::~Sprite() {
    SDL_DestroyTexture(texture);
    sprites.erase(id);
}

void Sprite::divideSheet(int columns, int rows) { 
    width = width / columns; 
    height = height / rows; 
    renderRect.w = width * SCALE;
    renderRect.h = height * SCALE;
    sourceRect = { 0, 0, width, height };
}

void Sprite::render() {
    if (!active)
        return;
    renderRect.x = (x - *cameraX - (width / 2)) * SCALE;
    renderRect.y = (y - *cameraY - height) * SCALE;
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
};

// STATIC

int Sprite::currentID = 0;

bool _comparePosition (Sprite* a, Sprite* b) {
    return a->y < b->y;
}
void Sprite::renderSprites() {
    vector<Sprite*> spriteList;
    for (auto const& [id, sprite] : Sprite::sprites){
        spriteList.push_back(sprite);
    }
    sort(spriteList.begin(), spriteList.end(), _comparePosition);
    for (auto sprite : spriteList){
        sprite->render();
    }
}