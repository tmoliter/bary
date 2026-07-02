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

Texture::~Texture() {
    if (texture)
        SDL_DestroyTexture(texture);
}

Sfx::Sfx(string n, string path) : name(n) {
    if (name == "") {
        sound = nullptr;
        return;
    }
    sound = Mix_LoadWAV(path.c_str());
}

Sfx::~Sfx() {
    if (sound)
        Mix_FreeChunk(sound);
}

shared_ptr<Texture> resourceDepository::getTexture(string name) {
    if (auto it = textures.find(name); it != textures.end())
        if (auto tex = it->second.lock())
            return tex;
    cout << "Cannot get texture " << name << " that is not loaded!\n";
    throw exception();
}

shared_ptr<Sfx> resourceDepository::getChunk(string name) {
    if (auto it = chunks.find(name); it != chunks.end())
        if (auto chunk = it->second.lock())
            return chunk;
    cout << "Cannot get chunk " << name << " that is not loaded!\n";
    throw exception();
}

shared_ptr<Texture> resourceDepository::loadTexture(string name, string path) {
    if (auto it = textures.find(name); it != textures.end())
        if (auto existing = it->second.lock())
            return existing;
    cout << name << " : " << path << endl;
    auto tex = make_shared<Texture>(name, path);
    textures[name] = tex;
    return tex;
}

void resourceDepository::loadTexturesFromTable(lua_State *L, map<string, shared_ptr<Texture>>& into) {
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        string name = lua_tostring(L, -2);
        string path = lua_tostring(L, -1);
        into[name] = loadTexture(name, path);
        lua_pop(L,1);
    }
    lua_pop(L,1);
}

resourceDepository::SceneResources resourceDepository::loadScene(lua_State *L) {
    SceneResources res;
    cout << "LOADING RESOURCES\n";
    if (!luaUtils::GetTableOnStackFromTable(L, "textures")) {
        cout << "no textures found\n";
        return res;
    }
    loadTexturesFromTable(L, res.textures);
    if (!luaUtils::GetTableOnStackFromTable(L, "sounds")) {
        cout << "no sounds found\n";
        return res;
    }
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        string name = lua_tostring(L, -2);
        string simplePath = lua_tostring(L, -1);
        string path = "assets/" + simplePath + ".mp3";
        if (auto it = chunks.find(name); it != chunks.end())
            if (auto existing = it->second.lock())
                res.chunks[name] = existing;
        if (!res.chunks.count(name)) {
            auto chunk = make_shared<Sfx>(name, path);
            chunks[name] = chunk;
            res.chunks[name] = chunk;
        }
        lua_pop(L,1);
    }
    lua_pop(L,1);
    return res;
}
