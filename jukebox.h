#ifndef JUKEBOX_H
#define JUKEBOX_H
#include <map>
#include <iostream>
#include <SDL2/SDL_mixer.h>

using namespace std;

struct SongDetails {
    SongDetails(string n, string path, double l);
    string name;
    double loopBeginning;
    Mix_Music* music;
};

namespace jukebox {
    inline SongDetails *currentSong = nullptr;

    inline map<string, SongDetails*> songs;

    SongDetails* initializeSong(string name);
    void loadSong(string name);
    void releaseSong(string name);

    void playSong(string name, bool loop);
    void stop();

    void releaseAll();
}

#endif
