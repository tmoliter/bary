#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H

#include <iostream>
#include <vector>
#include "RealThing.h"
#include "globals.h"

using namespace std;

class GhostFocus : public RealThing {
    public:
        RealThing* target;
        RealThing*& focus;
        GhostFocus(RealThing *&f, string targetName, bool usePos = false);

        bool usePosition;

        void shiftFocus() {};
        void destroy();

        void meat();

        void pan();

        static GhostFocus *g;
        static int create(RealThing *&f, string targetName, bool usePos = false);
};

#endif
