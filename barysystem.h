#ifndef BARYSYSTEM_H
#define BARYSYSTEM_H
#ifdef _WIN32
#include <direct.h>
#define cd _chdir
#else
#include "unistd.h"
#define cd chdir
#endif
#include "MapParser.h"
#include "jukebox.h"

namespace barysystem {
    void startup();
}

#endif