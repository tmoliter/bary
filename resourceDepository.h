#ifndef RESOURCE_DEPOSITORY_H
#define RESOURCE_DEPOSITORY_H
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "globals.h"

using namespace std;

struct Texture {
    Texture(string n, string path);
    ~Texture();
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    string name;
    SDL_Texture* texture = nullptr;
};


struct Sfx {
    Sfx(string n, string path);
    ~Sfx();
    Sfx(const Sfx&) = delete;
    Sfx& operator=(const Sfx&) = delete;
    string name;
    Mix_Chunk* sound = nullptr;
};


namespace resourceDepository {
    inline map<string, weak_ptr<Texture>> textures;
    inline map<string, weak_ptr<Sfx>> chunks;

    inline map<string, shared_ptr<Texture>> pinnedTextures;
    struct SceneResources {
        map<string, shared_ptr<Texture>> textures;
        map<string, shared_ptr<Sfx>> chunks;
    };

    shared_ptr<Texture> loadTexture(string name, string path);
    void loadTexturesFromTable(lua_State *L, map<string, shared_ptr<Texture>>& into);
    SceneResources loadScene(lua_State *L);

    shared_ptr<Texture> getTexture(string name);
    shared_ptr<Sfx> getChunk(string name);
};

struct Image {
    Image(string textureName, SDL_Rect sR) : sourceRect(sR), texture(resourceDepository::getTexture(textureName)) {}
    shared_ptr<Texture> texture;
    SDL_Rect sourceRect;
};

#endif
