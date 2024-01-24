#include "resourceDepository.h"

Texture::Texture(string n, string path) : name(n) {
    if (name == "") {
        texture = nullptr;
        return;
    }
    // pull from game name path here
    SDL_Surface* temp = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
}

Sfx::Sfx(string n, string path) : name(n) {
    if (name == "") {
        sound = nullptr;
        return;
    }
    // pull from game name path here
    sound = Mix_LoadWAV(path.c_str());
}

Texture* resourceDepository::initializeTexture(string name) {
    if (!resourceDepository::textureNameToPath.count(name)) {
        cout << "No texture with name " << name << "found\n";
        throw exception();
    }
    return new Texture(name, resourceDepository::textureNameToPath.at(name));
}

Sfx* resourceDepository::initializeSfx(string name) {
    if (!resourceDepository::soundNameToPath.count(name)) {
        cout << "No sound with name " << name << "found\n";
        throw exception();
    }
    return new Sfx(name, resourceDepository::soundNameToPath.at(name));
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

void resourceDepository::loadScene(lua_State *L) {
    cout << "LOADING RESOURCES\n";
    if (!luaUtils::GetTableOnStackFromTable(L, "textures")) {
        cout << "no textures found\n";
        return;
    }
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        string name = lua_tostring(L, -2);
        string path = lua_tostring(L, -1);
        resourceDepository::textureNameToPath[name] = path;
        lua_pop(L,1);
    }
    lua_pop(L,1);
    if (!luaUtils::GetTableOnStackFromTable(L, "sounds")) {
        cout << "no sounds found\n";
        return;
    }
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        string name = lua_tostring(L, -2);
        string path = lua_tostring(L, -1);
        resourceDepository::soundNameToPath[name] = path;
        lua_pop(L,1);
    }
    lua_pop(L,1);
}

void resourceDepository::endScene() {
    resourceDepository::textureNameToPath.clear();
    resourceDepository::soundNameToPath.clear();
}