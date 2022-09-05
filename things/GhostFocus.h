#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>
#include "globals.h"

using namespace std;

class GhostFocus : public Thing {
    public:
        Thing* target;
        Thing*& focus;
        GhostFocus(Thing *&f, string targetName);

        void shiftFocus() {};
        void destroy();

        void meat();

        void pan();

        static GhostFocus *g;
        static int create(Thing *&f, string targetName);
};

#endif
