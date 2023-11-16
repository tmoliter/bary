#ifndef BARYSYSTEM_H
#define BARYSYSTEM_H
#ifdef _WIN32
#include <direct.h>
#define cd _chdir
#else
#include "unistd.h"
#define cd chdir
#endif
#include "jukebox.h"
#include "globals.h"
#include <SDL2/SDL_image.h>

namespace barysystem {
    void startup();
}

#endif