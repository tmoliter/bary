#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : Thing(Point(0,0)) {
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
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        UIRenderer::setText(input);
    }
    if(keysDown.debug_return) {
        if (gameState == GameState::FieldFree) {
            input.clear();
            UIRenderer::clearText();
            gameState = GameState::TextInput;
            return;
        }
        if (gameState == GameState::TextInput) {
            input.clear();
            UIRenderer::clearText();
            gameState = GameState::FieldFree;
            return;
        }
    }
}