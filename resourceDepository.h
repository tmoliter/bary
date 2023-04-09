#ifndef RESOURCE_DEPOSITORY_H
#define RESOURCE_DEPOSITORY_H
#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "globals.h"

using namespace std;

namespace resourceDepository {
    inline map<string, pair<int, SDL_Texture*>> textures;
    inline map<string, pair<int, Mix_Chunk*>> chunks;

    SDL_Texture* getTexture(string path);
    void releaseTexture(string path);

    Mix_Chunk* getChunk(string path);
    void releaseChunk(string path);
};

#endif
