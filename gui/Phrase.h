#ifndef PHRASE_H
#define PHRASE_H
#include <map>
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Ray.h"

using namespace std;

class Phrase {
    public:
        int letterLength, letterHeight, phraseScale, progStart, advanceStart;
        Point &parent, offset;
        string text;
        queue<string> lines;
        queue<string> hiddenLines;

        Phrase(Point &p, Point o, int pixelWidth, int pixelHeight, int pS, string t);

        void progDisplay(int delay);

        void advance();

        static SDL_Texture *font;

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
