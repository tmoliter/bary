#ifndef PHRASE_H
#define PHRASE_H
#include <string>
#include <queue>
#include <cmath>
#include "Ray.h"
#include "Timer.h"
#include "resourceDepository.h"

using namespace std;

enum class ScrollType {
    oneLine,
    continuous,
    allButLast,
    preview,
};

enum class PhrasePhase {
    standby,
    appear,
    textDisplay,
};

class Phrase {
    public:
        string id;
        int maxLettersPerRow, maxRows, phraseScale, delay;
        int progStart, advanceStart, totalLines, percentDisplayed;
        Point gridLimits;
        int leftPad, topPad;
        bool fullyDisplayed;
        bool complete;
        bool autoDestroy;
        ScrollType scrollType;
        PhrasePhase state;
        SDL_Rect box;
        string text;
        queue<string> lines, hiddenLines;

        Texture* font;
        Texture* speechBubble;

        Phrase(Point p, Point pixelSize, ScrollType type, string t, Point gL = Point(1000,1000), int pS = 1, int d = 1);
        Phrase(const Phrase& ph);
        void reset();

        void advance();
        bool isComplete();

        int progDisplay();

        int appear();
        int disappear();

        SDL_Rect& getBox();
        void setGridLimits(DirectionMap dM);

        static int currentID;

    private:
        void popBubble(SDL_Rect sourceRect, float time);
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
