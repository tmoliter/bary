#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <string>
#include "./Phrase.h"
#include "./Text.h"
#include "./Line.h"
#include "./MenuDisplay.h"
#include "../Camera.h"

using namespace std;

enum class UiMode {
    MenuDisplay,
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
        vector<MenuDisplay*> menus;
        
        static void addPhrase(Phrase *p);
        static void addText(Text *t);
        static void addMenuDisplay(MenuDisplay *m);
        static void addLine(int &parentX, int &parentY, Ray* ray, LineType type);
        static void addLines(int &parentX, int &parentY, vector<Ray*> rays, LineType type);

        static void renderPhrases();
        static void renderTexts();
        static void renderLines();
        static void renderMenuDisplays();
        static void render();

        static void removePhrase(Phrase *p);
        static void removeText(Text *t);
        static void removeLine(Ray *r);
        static void removeMenuDisplay(MenuDisplay *m);
        static void changeLineType(Ray *r, LineType lineType);

        static int currentPhraseId;
        static UIRenderer *u;
};

#endif
