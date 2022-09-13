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

        // This is a hack to keep this pointer from going out of scope in the event, we should have a better way
        EventNode *node2;
};

#endif
