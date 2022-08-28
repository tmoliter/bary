#include "./things/Thing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

 void Thing::divideSheet(int columns, int rows) { 
        width = width / columns; 
        height = height / rows; 
        renderRect.w = width * SCALE;
        renderRect.h = height * SCALE;
        sourceRect = { 0, 0, width, height };
    }

void Thing::init()  {
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
            initialized = true;
        };

void Thing::render() {
        if (!initialized)
            return;
            
        renderRect.x = (x - *cameraX - (width / 2)) * SCALE;
        renderRect.y = (y - *cameraY - height) * SCALE;
        SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
    };

void Thing::incTick() {tick++;};

void Thing::destroy() {SDL_DestroyTexture(texture); delete this;};

int Thing::write_thing_datum(ifstream &mapData, ThingData &newTD) {
    int index = 0;
    string value = "";
    char current;
    while(mapData.get(current)) {
        if (current == ',') {
            switch(index) {
                case (0):
                    index++;
                    break;
                case (1):
                    index++;
                    newTD.id = std::stoi(value);
                    value.clear();
                    break;
                case (2):
                    index++;
                    newTD.x = std::stoi(value);
                    value.clear();
                    break;
                case (3):
                    index++;
                    newTD.y = std::stoi(value);
                    value.clear();
                    break;
                case (4):
                    newTD.path = strdup(value.c_str());
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
