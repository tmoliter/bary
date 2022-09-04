#include "./Camera.h"
#include <iostream>
#include "globals.h"
#include "./things/CameraEffect.h"


using namespace std;

void Camera::setPosition() {
    if (focusX == 0 || focusY == 0) {
        x = 0;
        y = 0;
    }
    int half_width =  SCREEN_WIDTH / SCALE / 2;
    int half_height = SCREEN_HEIGHT / SCALE / 2;

    if (*focusX < half_width)
        sourceRect.x = 0;
    else if (*focusX > width - half_width)
        sourceRect.x = width - (half_width* 2);
    else
        sourceRect.x = *focusX - half_width;

    if (*focusY < half_height)
        sourceRect.y = 0;
    else if (*focusY > height - half_height)
        sourceRect.y = height - (half_height * 2);
    else
        sourceRect.y = *focusY - half_height;

    x = sourceRect.x;
    y = sourceRect.y;
    // cout << x << "/" << *focusX << " : " << y <<  "/" << *focusY <<  endl;

    /* THIS IS A COOL 3D ANGLE SHIFT TO PLAY WITH LATER */
    // if (frameCount < 300)
    // {
    //     sourceRect.h += 1;
    //     if(frameCount % 5 == 0)
    //         sourceRect.w -= 1;
    // }
}

void Camera::init(int *fX, int *fY) {
    focusX = fX;
    focusY = fY;
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
    if(frameCount == 300) {
        new CameraEffect(*focusX,*focusY,1,EffectType::pan, 500);
    }
    if(frameCount == 600) {
        new CameraEffect(*focusX,*focusY,0,EffectType::pan, 500);
    }
    if (!initialized)
        return;
    // SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
}

void Camera::setFocus(int *fX, int *fY) {
    Camera::c->focusX = fX;
    Camera::c->focusY = fY;
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

