#ifndef PHRASE_H
#define PHRASE_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Ray.h"

using namespace std;

enum ScrollType {
    oneLine,
    continuous,
    allButLast,
};

class Phrase {
    public:
        ScrollType scrollType;
        bool fullyDisplayed;
        int letterLength, letterHeight, phraseScale, progStart, advanceStart, totalLines;
        Point &parent, offset;
        SDL_Rect box;
        string text;
        queue<string> lines;
        queue<string> hiddenLines;

        Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, int pS, ScrollType type, string t);

        int progDisplay(int delay);

        void advance();

        static SDL_Texture *font;

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
