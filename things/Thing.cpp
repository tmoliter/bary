#include "./things/Thing.h"
#include <fstream>
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

void Thing::render() {
        renderRect.x = (x - *cameraX - (width / 2)) * SCALE;
        renderRect.y = (y - *cameraY - height) * SCALE;
        SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
    };

void Thing::init(int *cX, int *cY, SDL_Renderer* r)  {
            renderer = r;
            cameraX = cX;
            cameraY = cY;
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
        };

void Thing::incTick() {tick++;};

void Thing::destroy() {SDL_DestroyTexture(texture); delete this;};

int Thing::write_thing_datum(ifstream &mapData, Thing *newThing) {
    ThingData td;
    int index = 0;
    string value = "";
    char current;
    while(mapData.peek() != EOF) {
        if (mapData.peek() == '\n') {
            newThing = new Thing(td);
            return 1;
        }
        mapData.get(current);
        if (current == ',') {
            switch(index) {
                case (0):
                    index++;
                    break;
                case (1):
                    index++;
                    td.id = std::stoi(value);
                    break;
                case (2):
                    index++;
                    td.x = std::stoi(value);
                    break;
                case (3):
                    index++;
                    td.y = std::stoi(value);
                    break;
                case (4):
                    index++;
                    td.path = value.c_str();
                    break;
                case (5):
                    newThing = new Thing(td);
                    return 1;
                default:
                    return 0;
                    break;
            }
            continue;
        }
        value.push_back(current);
    }
    newThing = new Thing(td);
    return 1;
}
