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
    // Bugs to fix:
    //  Saving to existing event breaks (maybe just work on SimpleMessage event child)
    //  Making a second thing with another SimpleMessage freezes when interacting
    //  Renaming a Thing needs to update everything in the EventMap db
    //  Triggers and simple message creates infinite loop. Maybe we don't care?
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

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
