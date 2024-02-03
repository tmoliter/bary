#include "./Camera.h"

using namespace std;

void Camera::setPosition() {
    setWarpLevel();
    int half_width =  sourceRect.w / 2;
    int half_height = sourceRect.h / 2;

    if (focalPoint.x < half_width)
        sourceRect.x = 0;
    else if (focalPoint.x > bgWidth - half_width)
        sourceRect.x = bgWidth - (half_width* 2);
    else
        sourceRect.x = focalPoint.x - half_width;

    if (focalPoint.y < half_height)
        sourceRect.y = 0;
    else if (focalPoint.y > bgHeight - half_height)
        sourceRect.y = bgHeight - (half_height * 2);
    else
        sourceRect.y = focalPoint.y - half_height;
}
Point Camera::getSourceRectCoords() {
    return Point(sourceRect.x, sourceRect.y);
}

void Camera::init() {
    bgTexture = resourceDepository::getTexture(bgTextureName)->texture;
    SDL_QueryTexture(bgTexture, NULL, NULL, &bgWidth, &bgHeight);
    sourceRect = { 0 , 0, scaledScreenWidth, scaledScreenHeight };
    renderRect = { 0 , 0, settings.SCREEN_WIDTH, settings.SCREEN_HEIGHT };
    fadeStart = warpStart = frameCount;
    fadeStatus = FxStatus::unapplying;
    focalPoint = Point(renderRect.w / 2, renderRect.h / 2);
    initialized = true;
}

void Camera::renderBackground() {
    if (!initialized)
        return;
    setPosition();
    SDL_RenderCopy(renderer, bgTexture, &sourceRect, &renderRect);
}

void Camera::renderAfterEffects() {
    handleFade();
}

void Camera::handleFade() {
    if(fadeStatus == FxStatus::unapplying || fadeStatus == FxStatus::applying)
        setOverlay();
    if(fadeStatus != FxStatus::unapplied)
        SDL_RenderFillRect(renderer, settings.RESOLUTION);
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

int Camera::fadeIn(int m) {
    if(c->fadeStatus == FxStatus::unapplied)
        return 1;
    if(c->fadeStatus == FxStatus::applied) {
        c->fadeMultiplier = m;
        c->fadeStatus = FxStatus::unapplying;
        c->fadeStart = frameCount;
    }
    return 0;
}

int Camera::fadeOut(int m) {
    if(c->fadeStatus == FxStatus::applied)
        return 1;
    if(c->fadeStatus == FxStatus::unapplied) {
        c->fadeMultiplier = m;
        c->fadeStatus = FxStatus::applying;
        c->fadeStart = frameCount;
    }
    return 0;
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

Point Camera::getPos() {
    return Point(c->sourceRect.x, c->sourceRect.y);
}

Point Camera::worldToScreen(Point p) {
    int renderX = (p.x - c->sourceRect.x) * settings.SCALE;
    int renderY = (p.y - c->sourceRect.y) * settings.SCALE;
    return Point(renderX, renderY);
}

Ray Camera::worldToScreen(Ray r) {
    Point renderA = worldToScreen(r.a);
    Point renderB = worldToScreen(r.b);
    return Ray(renderA, renderB);
}
