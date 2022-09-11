#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./Phrase.h"
#include "../Camera.h"

using namespace std;

enum UiMode {
    Menu,
    Dialog,
    DisplayOnly,
    Hidden
};

class UIManager {
    public:
        UiMode mode;

        // Maybe this should be a linked list or something
        map<int, Phrase*> phrases;
        vector<int> phrasesToDelete;
        int activePhrase;

        static void meat(KeyPresses keysDown);
        static void addPhrase(Point o, int pixelWidth, int pixelHeight, int scale, ScrollType type, string t, bool setAsActive);
        static void renderPhrases();
        static void cleanUp();

        static int currentPhraseId;
        static UIManager *u;
};

#endif
