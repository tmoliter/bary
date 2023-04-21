#include "jukebox.h"

Song::Song(string n, string path, double l) : name(n), loopBeginning(l) {
    if (name == "") {
        music = nullptr;
        return;
    }
    music = Mix_LoadMUS(path.c_str());
}


Song* jukebox::initializeSong(string name) {
    if (name == "Boss Battle") {
        return new Song(
            "Boss Battle",
            "assets/music/boss-battle.mp3XXX",
            11.576
        );
    }
    return nullptr;
}

void jukebox::playSong(string name, bool loop) {
    loadSong(name);
    stop();
    if (songs.count(name) < 1)
        return;
    currentSong = songs[name];
    Mix_PlayMusic(currentSong->music, 0);
    if (loop)
        Mix_HookMusicFinished([]() {
            Mix_PlayMusic(currentSong->music, 0);
            Mix_SetMusicPosition(currentSong->loopBeginning);
        });
}

void jukebox::stop() {
    if (currentSong == nullptr)
        return;
    Mix_HookMusicFinished(NULL);
    Mix_HaltMusic();
    currentSong = nullptr;
}

void jukebox::loadSong(string name) {
    if(songs.count(name))
        return;
    Song* song = initializeSong(name);
    if (song == nullptr)
        return;
    if (song->music == nullptr) {
        delete song;
        return;
    }
    songs[name] = song;
}


void jukebox::releaseSong(string name) {
    if (!songs.count(name))
        return;
    Mix_FreeMusic(songs[name]->music);
    delete songs[name];
    songs.erase(name);
}

void jukebox::releaseAll() {
   map<string, Song*>::iterator itr = songs.begin();
   while (itr != songs.end()) {
        Mix_FreeMusic(itr->second->music);
        delete itr->second;
        itr = songs.erase(itr);
   }
}