#include <vector>
#include <iostream>
#include "Sprite.h"

using namespace std;

Sprite::Sprite (int &x, int &y, int &tI, SpriteData sd) : 
x(x), 
y(y), 
thingId(tI), 
layer(sd.layer),
xOffset(sd.xOffset),
yOffset(sd.yOffset),
path(sd.path),
active(false) {
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

int Sprite::write_sprite_datum(ifstream &mapData, SpriteData &newSD){
    int index = 0;
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',') {
            switch(index) {
                case (0):
                    index++;
                    newSD.layer = std::stoi(value);
                    value.clear();
                    break;
                case (1):
                    index++;
                    newSD.xOffset = std::stoi(value);
                    value.clear();
                    break;
                case (2):
                    index++;
                    newSD.yOffset = std::stoi(value);
                    value.clear();
                    break;
                case (3):
                    newSD.path = strdup(value.c_str());
                    value.clear();
                    return 1;
                default:
                    return 0;
            }
            continue;
        }
        value.push_back(current);
    }
    return 1;
}