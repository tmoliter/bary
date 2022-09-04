#include "./Camera.h"
#include <iostream>
#include "globals.h"
#include "./things/GhostFocus.h"


using namespace std;

void Camera::setPosition() {
    int half_width =  SCREEN_WIDTH / SCALE / 2;
    int half_height = SCREEN_HEIGHT / SCALE / 2;

    int fX,fY;
    focus->getCenter(fX,fY);

    if (fX < half_width)
        sourceRect.x = 0;
    else if (fX > width - half_width)
        sourceRect.x = width - (half_width* 2);
    else
        sourceRect.x = fX - half_width;

    if (fY < half_height)
        sourceRect.y = 0;
    else if (fY > height - half_height)
        sourceRect.y = height - (half_height * 2);
    else
        sourceRect.y = fY - half_height;

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

void Camera::init(Thing *f) {
    focus = f;
    SDL_Surface* temp = IMG_Load(path);
    bgTexture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    SDL_QueryTexture(bgTexture, NULL, NULL, &width, &height);
    sourceRect = { 0 , 0, SCREEN_WIDTH / SCALE, SCREEN_HEIGHT / SCALE };
    renderRect = { 0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    initialized = true;
}

void Camera::render() {
    setPosition();

    /* TEST STUFF */
    if(frameCount == 300) {
        new GhostFocus(focus,1,EffectType::pan, 500);
    }
    if(frameCount == 600) {
        new GhostFocus(focus,0,EffectType::pan, 500);
    }
    /* TEST STUFF */
    if (!initialized)
        return;
    // SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
    Sprite::renderSprites(renderer, Point(x,y));
}

void Camera::setFocus(Thing* f) {
    Camera::c->focus = f;
}

int Camera::parse_camera(ifstream &mapData) {
    char current;
    string value = "";
    mapData.get();
    while (mapData.get(current)) {
        if(current == ',') {
            Camera::c->path = strdup(value.c_str());
            return 1;
        }
        value.push_back(current);
    }
    return 1;
}

