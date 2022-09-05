#include "./Camera.h"
#include <iostream>
#include "globals.h"
#include "./things/GhostFocus.h"


using namespace std;

void Camera::setPosition() {
    setWarpLevel();
    int half_width =  sourceRect.w / 2;
    int half_height = sourceRect.h / 2;
    Point fp = focus->getCenter();

    if (fp.x < half_width)
        sourceRect.x = 0;
    else if (fp.x > bgWidth - half_width)
        sourceRect.x = bgWidth - (half_width* 2);
    else
        sourceRect.x = fp.x - half_width;

    if (fp.y < half_height)
        sourceRect.y = 0;
    else if (fp.y > bgHeight - half_height)
        sourceRect.y = bgHeight - (half_height * 2);
    else
        sourceRect.y = fp.y - half_height;
}

void Camera::init(Thing *f) {
    focus = f;
    SDL_Surface* temp = IMG_Load(path);
    bgTexture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    SDL_QueryTexture(bgTexture, NULL, NULL, &bgWidth, &bgHeight);
    sourceRect = { 0 , 0, scaledScreenWidth, scaledScreenHeight };
    renderRect = { 0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    fadeStart = warpStart = frameCount;
    fadeStatus = FxStatus::unapplying;
    initialized = true;
}

void Camera::render() {
    setPosition();
    if (!initialized)
        return;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
    Sprite::renderSprites(renderer, Point(sourceRect.x,sourceRect.y));
    handleFade();
}

void Camera::handleFade() {
    if(fadeStatus == FxStatus::unapplying || fadeStatus == FxStatus::applying)
        setOverlay();
    if(fadeStatus != FxStatus::unapplied)
        SDL_RenderFillRect(renderer, FULL_SCREEN);
};

void Camera::setOverlay() {
    int t = (frameCount - fadeStart) * fadeMultiplier;
    utils::limit(t, 0, 255);
    int a = fadeStatus == FxStatus::unapplying ? 255 - t : t;
    SDL_SetRenderDrawColor(renderer,0,0,0,a);
    if (t < 255)
        return;
    if(fadeStatus == FxStatus::unapplying)
        fadeStatus = FxStatus::unapplied;
    if(fadeStatus == FxStatus::applying)
        fadeStatus = FxStatus::applied;
}

void Camera::setWarpLevel() {
    /* THIS IS A COOL 3D ANGLE SHIFT TO PLAY WITH LATER */
    // if (frameCount < 300)
    // {
    //     sourceRect.h += 1;
    //     if(frameCount % 5 == 0)
    //         sourceRect.w -= 1;
    // }
    if(warpStatus == FxStatus::unapplied || warpStatus == FxStatus::applied)
        return;
    int t = (frameCount - warpStart) * warpMultiplier;
    int newWidth = warpStatus == FxStatus::applying ? scaledScreenWidth - t : t;
    int newHeight = warpStatus == FxStatus::applying ? scaledScreenHeight - t : t;
    utils::limit(newWidth,1,scaledScreenWidth);
    utils::limit(newHeight,1,scaledScreenHeight);
    sourceRect.w = newWidth;
    sourceRect.h = newHeight;
    if (t < scaledScreenHeight || t < scaledScreenWidth) {
        return;
    }
    if(warpStatus == FxStatus::unapplying)
        warpStatus = FxStatus::unapplied;
    if(warpStatus == FxStatus::applying)
        warpStatus = FxStatus::applied;
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
    if(c->fadeStatus == FxStatus::unapplied) {
        c->fadeMultiplier = m;
        c->fadeStatus = FxStatus::applying;
        c->fadeStart = frameCount;
    }
}

void Camera::fadeOut(int m) {
    if(c->fadeStatus == FxStatus::applied) {
        c->fadeMultiplier = m;
        c->fadeStatus = FxStatus::unapplying;
        c->fadeStart = frameCount;
    }
}

void Camera::warpIn(int m) {
    if(c->warpStatus == FxStatus::unapplied) {
        c->warpMultiplier = m;
        c->warpStatus = FxStatus::applying;
        c->warpStart = frameCount;
    }
}

void Camera::warpOut(int m) {
    if(c->warpStatus == FxStatus::applied) {
        c->warpMultiplier = m;
        c->warpStatus = FxStatus::unapplying;
        c->warpStart = frameCount;
    }
}

string Camera::getFocusName() {
    return c->focus->name;
}
