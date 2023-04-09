#include "resourceDepository.h"

SDL_Texture* resourceDepository::getTexture(string path) {
    if(!textures.count(path)) {
        SDL_Surface* temp = IMG_Load(path.c_str());
        textures[path].second = SDL_CreateTextureFromSurface(renderer, temp);
        textures[path].first = 1;
        SDL_FreeSurface(temp);
    } else {
        textures[path].first++;
    }
    return textures[path].second;
}

void resourceDepository::releaseTexture(string path) {
    if (textures.count(path) < 1)
        return;
    if (--textures[path].first < 1) {
        SDL_DestroyTexture(textures[path].second);
        textures.erase(path);
    }
}

Mix_Chunk* resourceDepository::getChunk(string path) {
    if(!chunks.count(path)) {
        chunks[path].second = Mix_LoadWAV(path.c_str());
        chunks[path].first = 1;
    } else {
        chunks[path].first++;
    }
    return chunks[path].second;
}

void resourceDepository::releaseChunk(string path) {
    if (chunks.count(path) < 1)
        return;
    if (--chunks[path].first < 1) {
        Mix_FreeChunk(chunks[path].second);
        chunks.erase(path);
    }
}