#ifndef RESOURCE_DEPOSITORY_H
#define RESOURCE_DEPOSITORY_H
#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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

    Sfx* initializeSfx(string name);
    Texture* initializeTexture(string name);

    // Maybe have prefetch functions that take a list of names and loads all of them, and releases them after

    Texture* getTexture(string name);
    void releaseTexture(string name);

    Sfx* getChunk(string name);
    void releaseChunk(string name);
};

#endif
