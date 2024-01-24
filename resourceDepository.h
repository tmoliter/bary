#ifndef RESOURCE_DEPOSITORY_H
#define RESOURCE_DEPOSITORY_H
#include <map>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "globals.h"

using namespace std;

struct Texture {
    Texture(string n, string path);
    string name;
    SDL_Texture* texture;
};


struct Sfx {
    Sfx(string n, string path);
    string name;
    Mix_Chunk* sound;
};


namespace resourceDepository {
    inline map<string, pair<int, Texture*>> textures;
    inline map<string, pair<int, Sfx*>> chunks;

    void loadScene(lua_State *L);
    void removeUnreferencedTextures();

    // Maybe have prefetch functions that take a list of names and loads all of them, and releases them after

    Texture* getTexture(string name);
    void releaseTexture(string name);

    Sfx* getChunk(string name);
    void releaseChunk(string name);
};

struct Image {
    Image(string textureName, SDL_Rect sR) : sourceRect(sR) {
        texture = resourceDepository::getTexture(textureName);
    }
    ~Image() {
        resourceDepository::releaseTexture(texture->name);
    }
    Texture* texture;
    SDL_Rect sourceRect;
};

#endif
