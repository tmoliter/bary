#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "./Phrase.h"
#include "./Text.h"
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
        vector<Text*> texts;

        static void addPhrase(Phrase *p);
        static void addText(Text *t);

        static void renderPhrases();
        static void renderTexts();
        static void render();

        static void removePhrase(Phrase *p);
        static void removeText(Text *t);

        static int currentPhraseId;
        static UIRenderer *u;
};

#endif
