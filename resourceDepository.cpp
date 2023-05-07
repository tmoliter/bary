#include "resourceDepository.h"

Texture::Texture(string n, string path) : name(n) {
    if (name == "") {
        texture = nullptr;
        return;
    }
    SDL_Surface* temp = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

Sfx::Sfx(string n, string path) : name(n) {
    if (name == "") {
        sound = nullptr;
        return;
    }
    sound = Mix_LoadWAV(path.c_str());
}

Texture* resourceDepository::initializeTexture(string name) {
    if (name == "sailorshack")
        return new Texture( "sailorshack", "assets/sheets/Burg/SailorShack.png");
    if (name == "genrl")
        return new Texture("genrl", "assets/x.png");
    if (name == "zinnia")
        return new Texture("zinnia", "assets/sheets/SDL_TestSS.png");
    if (name == "editorCross")
        return new Texture("editorCross", "assets/debug/9x9cross.png");
    if (name == "singlepixel")
        return new Texture("singlepixel", "assets/debug/onePixel.png");
    if (name == "pinkbox")
        return new Texture("pinkbox", "assets/menus/blankPink.png");
    if (name == "pinkinventoryheader")
        return new Texture("pinkbox", "assets/menus/pinkInventoryHeader.png");
    return nullptr;
}

Sfx* resourceDepository::initializeSfx(string name) {
    if (name == "fart")
        return new Sfx("fart", "assets/sfx/fart.mp3");
    return nullptr;
}

Texture* resourceDepository::getTexture(string name) {
    if(!textures.count(name)) {
        textures[name].second = initializeTexture(name);
        textures[name].first = 1;
    } else {
        textures[name].first++;
    }
    return textures[name].second;
}

void resourceDepository::releaseTexture(string name) {
    if (textures.count(name) < 1)
        return;
    if (--textures[name].first < 1) {
        SDL_DestroyTexture(textures[name].second->texture);
        delete textures[name].second;
        textures.erase(name);
    }
}

Sfx* resourceDepository::getChunk(string name) {
    if(!chunks.count(name)) {
        chunks[name].second = initializeSfx(name);
        chunks[name].first = 1;
    } else {
        chunks[name].first++;
    }
    return chunks[name].second;
}

// Maybe this should take a Sfx pointer
void resourceDepository::releaseChunk(string name) {
    if (chunks.count(name) < 1)
        return;
    if (--chunks[name].first < 1) {
        Mix_FreeChunk(chunks[name].second->sound);
        delete chunks[name].second;
        chunks.erase(name);
    }
}