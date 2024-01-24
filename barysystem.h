#ifndef BARYSYSTEM_H
#define BARYSYSTEM_H
#include "jukebox.h"
#include "globals.h"
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;
namespace barysystem {
    void startup(vector<string>& saveNames);
}

#endif