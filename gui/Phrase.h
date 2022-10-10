#ifndef PHRASE_H
#define PHRASE_H
#include <string>
#include <queue>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Ray.h"

using namespace std;

enum class ScrollType {
    oneLine,
    continuous,
    allButLast,
    preview,
};

class Phrase {
    public:
        int maxLettersPerRow, maxRows, phraseScale, delay;
        int progStart, advanceStart, totalLines;

        Point gridLimits;
        int leftPad, topPad;
        bool fullyDisplayed;
        bool complete;
        bool autoDestroy;
        ScrollType scrollType;
        SDL_Rect box;
        string text;
        queue<string> lines, hiddenLines;

        Phrase(Point p, Point pixelSize, ScrollType type, string t, Point gL = Point(1000,1000), int pS = 1, int d = 1);
        Phrase(const Phrase& ph);
        void reset();

        void advance();
        bool isComplete();

        int progDisplay();

        SDL_Rect& getBox();
        void setGridLimits(DirectionMap dM);

        static inline SDL_Texture *defaultSpeechBubble = nullptr;

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
