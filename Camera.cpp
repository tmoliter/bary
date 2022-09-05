#include "./Camera.h"
#include <iostream>
#include "globals.h"
#include "./things/GhostFocus.h"


using namespace std;

void Camera::setPosition() {
    int half_width =  SCREEN_WIDTH / SCALE / 2;
    int half_height = SCREEN_HEIGHT / SCALE / 2;
    Point fp = focus->getCenter();

    if (fp.x < half_width)
        sourceRect.x = 0;
    else if (fp.x > width - half_width)
        sourceRect.x = width - (half_width* 2);
    else
        sourceRect.x = fp.x - half_width;

    if (fp.y < half_height)
        sourceRect.y = 0;
    else if (fp.y > height - half_height)
        sourceRect.y = height - (half_height * 2);
    else
        sourceRect.y = fp.y - half_height;

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
    fadeStart = frameCount;
    fadeStatus = FadeStatus::unfading;
    initialized = true;
}

void Camera::render() {
    setPosition();
    if (!initialized)
        return;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
    Sprite::renderSprites(renderer, Point(sourceRect.x,sourceRect.y));
    
    if(fadeStatus == FadeStatus::unfading || fadeStatus == FadeStatus::fading)
        setOverlay();
    if(fadeStatus != FadeStatus::visible)
        SDL_RenderFillRect(renderer, FULL_SCREEN);
}

void Camera::setOverlay() {
    int t = (frameCount - fadeStart) * fadeMultiplier;
    if (t > 255)
        t = 255;
    int a = fadeStatus == FadeStatus::unfading ? 255 - t : t;
    cout << a << endl;
    SDL_SetRenderDrawColor(renderer,0,0,0,a);
    if (t < 255)
        return;
    if(fadeStatus == FadeStatus::unfading)
        fadeStatus = FadeStatus::visible;
    if(fadeStatus == FadeStatus::fading)
        fadeStatus = FadeStatus::faded;
}

// STATIC

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

void Camera::panTo(string thingName) {
    GhostFocus::create(c->focus, thingName);
}

void Camera::fadeIn(int m) {
    if(c->fadeStatus == FadeStatus::visible) {
        c->fadeMultiplier = m;
        c->fadeStatus = FadeStatus::fading;
        c->fadeStart = frameCount;
    }
}

void Camera::fadeOut(int m) {
    if(c->fadeStatus == FadeStatus::faded) {
        c->fadeMultiplier = m;
        c->fadeStatus = FadeStatus::unfading;
        c->fadeStart = frameCount;
    }
}


string Camera::getFocusName() {
    return c->focus->name;
}
