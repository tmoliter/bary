#ifndef TEXT_H
#define TEXT_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Ray.h"

using namespace std;

class Text {
    public:
        Point position;
        string text;

        Text(Point p, string t);

        void render();
        void setText(string newCopy);
        void clearText();
        void setPos(Point p);

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
