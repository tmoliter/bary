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
void resourceDepository::loadTexture(string name, string simplePath) {
    cout << simplePath << endl;
    if(!textures.count(name))
        textures[name] = make_pair(0,new Texture(name, simplePath));
    else
        cout << "texture " << name << " is already loaded, skipping\n";
}

Texture* resourceDepository::getTexture(string name) {
    if(!textures.count(name)) {
        cout << "Cannot get texture " << name << " that is not loaded!\n";
        throw exception();
    }
    textures[name].first++;
    return textures[name].second;
}

void resourceDepository::releaseTexture(string name) {
    if(!textures.count(name)) {
        cout << "Cannot release texture " << name << " that is not loaded!\n";
        throw exception();
    }
    textures[name].first--;
}

Sfx* resourceDepository::getChunk(string name) {
    if(!chunks.count(name)) {
        cout << "Cannot get chunk " << name << " that is not loaded!\n";
        throw exception();
    }
    chunks[name].first++;
    return chunks[name].second;
}

// Maybe this should take a Sfx pointer
void resourceDepository::releaseChunk(string name) {
    if(!chunks.count(name)) {
        cout << "Cannot release sound chunk " << name << " that is not loaded!\n";
        throw exception();
    }
    chunks[name].first--;
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
        string simplePath = lua_tostring(L, -1);
        loadTexture(name, simplePath);
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
        string simplePath = lua_tostring(L, -1);
        string path = "assets/" + simplePath + ".mp3";
        if(!chunks.count(name))
            chunks[name] = make_pair(0,new Sfx(name, path));
        else
            cout << "sound chunk " << name << " is already loaded, skipping\n";
        lua_pop(L,1);
    }
    lua_pop(L,1);
}

void resourceDepository::removeUnreferencedTextures() { // this shits broken
    map<string, pair<int, Texture*>>::iterator tItr = textures.begin();
    while (tItr != textures.end()) {
        string name = tItr->first;
        int references = tItr->second.first;
        Texture* texture = tItr->second.second;
        if (references > 0) {
            cout << "will not destroy texture " << name << " that has " << references << " references!\n";
            tItr++;
            continue;
        }
        SDL_DestroyTexture(textures[name].second->texture);
        delete textures[name].second;
        textures.erase(name);
    }

    map<string, pair<int, Sfx*>>::iterator sItr = chunks.begin();
    while (sItr != chunks.end()) {
        string name = sItr->first;
        int references = sItr->second.first;
        Sfx* texture = sItr->second.second;
        if (references > 0) {
            cout << "will not destroy sound chunk " << name << " that has " << references << " references!\n";
            sItr++;
            continue;
        }
        Mix_FreeChunk(chunks[name].second->sound);
        delete chunks[name].second;
        chunks.erase(name);
    }
}