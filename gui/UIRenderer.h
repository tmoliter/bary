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
#include "./Line.h"
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
        vector<Line*> lines;
        
        static void addPhrase(Phrase *p);
        static void addText(Text *t);
        static void addLines(int &parentX, int &parentY, vector<Ray*> rays, LineType type);

        static void renderPhrases();
        static void renderTexts();
        static void renderLines();
        static void render();

        static void removePhrase(Phrase *p);
        static void removeText(Text *t);
        static void clearLines();

        static int currentPhraseId;
        static UIRenderer *u;
};

#endif
