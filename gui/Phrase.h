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
        int letterLength, letterHeight, phraseScale, delay;
        int progStart, advanceStart, totalLines;
        bool fullyDisplayed;
        bool complete;
        bool autoDestroy;
        ScrollType scrollType;
        Point position;
        SDL_Rect box;
        string text;
        queue<string> lines, hiddenLines;

        Phrase(Point o, int pixelWidth, int pixelHeight, int pS, ScrollType type, string t, int d = 1, bool aD = false);

        void advance();
        bool isComplete();

        int progDisplay();
        void reset();


        static SDL_Texture *font;

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
