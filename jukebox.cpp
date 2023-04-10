#include "jukebox.h"

SongDetails::SongDetails(string n, string path, double l) : name(n), loopBeginning(l) {
    if (name == "") {
        music = nullptr;
        return;
    }
    music = Mix_LoadMUS(path.c_str());
}


SongDetails* jukebox::initializeSong(string name, int& success) {
    if (name == "Boss Battle") {
        success = 1;
        return new SongDetails(
            "Boss Battle",
            "assets/music/boss-battle.mp3",
            11.576
        );
    }
    return nullptr;
}

void jukebox::playSong(string name, bool loop, bool load) {
    if (load)
        loadSong(name);
    stop(false);
    if (songs.count(name) < 1)
        return;
    currentSong = songs[name].second;
    Mix_PlayMusic(currentSong->music, 0);
    if (loop) {
        double x = 0;
        Mix_HookMusicFinished([]() {
            Mix_PlayMusic(currentSong->music, 0);
            Mix_SetMusicPosition(currentSong->loopBeginning);
        });
    }
}

void jukebox::stop(bool release) {
    if (currentSong == nullptr)
        return;
    Mix_HookMusicFinished(NULL);
    Mix_HaltMusic();
    if (release)
        releaseSong(currentSong->name);
    currentSong = nullptr;
}

void jukebox::loadSong(string name) {
    if(!songs.count(name)) {
        int songExists = 0;
        songs[name].second = initializeSong(name, songExists);
        if (songExists == 0) {
            songs.erase(name);
            return;
        }
        songs[name].first = 1;
        return;
    }
    songs[name].first++;
}

void jukebox::releaseSong(string name) {
    if (songs.count(name) < 1)
        return;
    if (--songs[name].first < 1) {
        Mix_FreeMusic(songs[name].second->music);
        delete songs[name].second;
        songs.erase(name);
    }
}