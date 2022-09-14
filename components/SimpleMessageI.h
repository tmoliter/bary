#ifndef SIMPLE_MESSAGE_I_H
#define SIMPLE_MESSAGE_I_H
#include "Event.h"
#include "Timer.h"
#include "./Interactable.h"
#include "../things/FieldPlayer.h"

class SimpleMessageI : public Interactable {
    public:
        SimpleMessageI(Thing *parent, string message, CollidableData cd);
        ~SimpleMessageI();
};

#endif
