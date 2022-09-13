#ifndef SIMPLE_MESSAGE_I_H
#define SIMPLE_MESSAGE_I_H
#include "./Interactable.h"
#include "Event.h"
#include "Timer.h"
#include "../things/FieldPlayer.h"
#include <map>

class SimpleMessageI : public Interactable {
    public:
        SimpleMessageI(Thing *parent, string message, CollidableData cd);
        ~SimpleMessageI();
};

#endif
