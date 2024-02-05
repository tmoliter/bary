#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "Ray.h"
#include "resourceDepository.h"

using namespace std;

class Text {
    public:
        Point position;
        string text;
        int lineLength;

        Texture* font;

        Text() : text(""), lineLength(-1) {
            font = resourceDepository::getTexture("defaultFont");
        };
        Text(Point p, string t, int lL = -1);

        void render();
        void setText(string newCopy);
        void setLineLengthFromPixelWidth(int pixelWidth);
        void clearText();
        void setPos(Point p);

        void resetLineLength();

    private:
        void renderLetter(int lineNumber, int position, int asciiValue, int occlusion, int raise);
};

#endif
