#ifndef CAMERA_EFFECT_H
#define CAMERA_EFFECT_H

#include "Thing.h"
#include "../components/Walk.h"
#include "../components/Obstruction.h"
#include <iostream>
#include <vector>

using namespace std;

enum EffectType {
    shake,
    pan
};

class CameraEffect : public Thing {
    public:
        int *targetX, *targetY, start, duration;
        EffectType type;
        CameraEffect(int fX, int fY, int targetId, EffectType t, int d);

        void shiftFocus() {};
        void destroy();

        void meat();

        void pan();
        void shake();

        Sprite* overlay;
};

#endif
