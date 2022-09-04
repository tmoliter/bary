#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>
#include "globals.h"

using namespace std;

enum EffectType {
    shake,
    pan
};

class GhostFocus : public Thing {
    public:
        int start, duration;
        Thing* target;
        Thing* focus;
        EffectType type;
        GhostFocus(Thing *&focus, int targetId, EffectType t, int d);

        void shiftFocus() {};
        void destroy();

        void meat();

        void pan();
        void shake();

        Sprite* overlay;
};

#endif
