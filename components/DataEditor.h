#ifndef DATA_EDITOR_H
#define DATA_EDITOR_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Thing.h"
#include "globals.h"
#include "../components/Sprite.h"
#include "../gui/UIRenderer.h"

using namespace std;

struct DataEditor {
    // Editable things and components have getInts etc. functions that populate this
    // Has different modes for scrolling through and editing values of each data type
    // and a button to switch between data type buckets
    vector<int*> ints;
    vector<string*> strings;
};

#endif
