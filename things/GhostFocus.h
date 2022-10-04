#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H

#include <iostream>
#include <vector>
#include "Thing.h"
#include "globals.h"

using namespace std;

class GhostFocus : public Thing {
    public:
        Thing* target;
        Thing*& focus;
        GhostFocus(Thing *&f, string targetName, bool usePos = false);

        bool usePosition;

        void shiftFocus() {};
        void destroy();

        void meat();

        void pan();

        static GhostFocus *g;
        static int create(Thing *&f, string targetName, bool usePos = false);
};

#endif
