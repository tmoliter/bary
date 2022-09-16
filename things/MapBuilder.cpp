#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : Thing(Point(0,0)), editSpeed(1) {
    SpriteData sD;
    sD.height = 0;
    sD.width = 0;
    sD.path = "assets/debug/onePixel.png";
    sprite = new Sprite(position.x, position.y ,name, sD);

    if (!Phrase::font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        Phrase::font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }

    mapBuilder = this;
}

void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (keysDown.up) 
        position.y-=3;
    if (keysDown.down)
        position.y+=3;
    if (keysDown.left)
        position.x-=3;
    if (keysDown.right)
        position.x+=3;
    if (keysDown.debug_up) 
        position.y--;
    if (keysDown.debug_down)
        position.y++;
    if (keysDown.debug_left)
        position.x--;
    if (keysDown.debug_right)
        position.x++;

    if(keysDown.debug_plus)
        editSpeed++;
    if(keysDown.debug_minus && editSpeed > 1)
        editSpeed--;

    if (keysDown.debug_1)
        sprite->sourceRect.x -= editSpeed;
    if (keysDown.debug_2)
        sprite->sourceRect.x += editSpeed;
    if (keysDown.debug_3)
        sprite->sourceRect.y -= editSpeed;
    if (keysDown.debug_4)
        sprite->sourceRect.y += editSpeed;
    if (keysDown.debug_5){
        sprite->sourceRect.w -= editSpeed;
        sprite->renderRect.w -= editSpeed;
    }
    if (keysDown.debug_6){
        sprite->sourceRect.w += editSpeed;
        sprite->renderRect.w += editSpeed;
    }
    if (keysDown.debug_7){
        sprite->sourceRect.h -= editSpeed;
        sprite->renderRect.h -= editSpeed;
    }
    if (keysDown.debug_8){
        sprite->sourceRect.h += editSpeed;
        sprite->renderRect.h += editSpeed;
    }


    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        UIRenderer::setText(input);
        return;
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        UIRenderer::setText(input);
        return;
    }
    if(keysDown.debug_return) {
        if (gameState == GameState::FieldFree) {
            input.clear();
            UIRenderer::clearText();
            gameState = GameState::TextInput;
            return;
        }
        if (gameState == GameState::TextInput) {
            setSprite();
            input.clear();
            UIRenderer::clearText();
            gameState = GameState::FieldFree;
            return;
        }
    }
}

void MapBuilder::setSprite() {
    string possiblePath = string(BASE_PATH) + "assets/" + input;
    const char* cPossiblePath = possiblePath.c_str();
    ifstream f(cPossiblePath);
    if(!f.good())
        return;
    string path = "./assets/" + input;
    SpriteData sd;
    sd.path = path.c_str();
    Sprite* next = new Sprite(position.x, position.y, name, sd);
    delete sprite;
    sprite = next;
}