#include <vector>
#include <iostream>
#include "Sprite.h"

using namespace std;

Sprite::Sprite (int &x, int &y, int &tI, SpriteData sd) : 
x(x), 
y(y), 
thingId(tI), 
layer(sd.layer),
renderOffset(sd.renderOffset),
xOffset(sd.xOffset),
yOffset(sd.yOffset),
texture(sd.texture),
active(false) {
    id = currentID++;
    Sprite::sprites[id] = this;
    renderer = Camera::c->renderer;
    cameraX = &Camera::c->x;
    cameraY = &Camera::c->y;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    if(sd.width > 0)
        width = sd.width;
    if(sd.height > 0)
        height = sd.height;
    renderRect = {
        -1000,
        -1000,
        width * SCALE, 
        height * SCALE 
    };
    sourceRect = { sd.sourceX, sd.sourceY, width, height };
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
    renderRect.x = ((x - *cameraX) + xOffset) * SCALE;
    renderRect.y = ((y - *cameraY) + yOffset) * SCALE;
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
};

// STATIC

int Sprite::currentID = 0;

bool _comparePosition (Sprite* a, Sprite* b) {
    if (a->layer == b->layer)
        return (
            a->y + a->height + a->yOffset - a->renderOffset < 
            b->y + b->height + b->yOffset - b->renderOffset
        );
    return a->layer < b->layer;
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
        while (current == '|') {
            cout << "skipping " << index << endl;
            index++;
            mapData.get(current);
        }
        if (current == '*') {
            cout << "skipping remainder" << endl;
            index = 8;
            mapData.get(current);
        }
        if (current == ',') {
            switch(index) {
                case (0):
                    cout << value << endl;
                    newSD.layer = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (1):
                    cout << value << endl;
                    newSD.renderOffset = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (2):
                    cout << value << endl;
                    newSD.width = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (3):
                    cout << value << endl;
                    newSD.height = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (4):
                    cout << value << endl;
                    newSD.sourceX = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (5):
                    cout << value << endl;
                    newSD.sourceY = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (6):
                    cout << value << endl;
                    newSD.xOffset = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (7):
                    cout << value << endl;
                    newSD.yOffset = std::stoi(value);
                    value.clear();
                    index++;
                    break;
                case (8):
                    cout << value << endl << endl;
                    if(!textures.count(value)) {
                        SDL_Surface* temp = IMG_Load(value.c_str());
                        textures[value] = SDL_CreateTextureFromSurface(Camera::c->renderer, temp);
                        SDL_FreeSurface(temp);
                    }
                    newSD.texture = textures[value];
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