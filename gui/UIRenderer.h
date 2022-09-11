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

class UIRenderer {
    public:
        UiMode mode;

        vector<Phrase*> phrases;

        static void addPhrase(Phrase *p);
        static void renderPhrases();
        static void removePhrase(Phrase *p);

        static int currentPhraseId;
        static UIRenderer *u;
};

#endif
