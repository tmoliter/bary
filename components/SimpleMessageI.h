#ifndef SIMPLE_MESSAGE_I_H
#define SIMPLE_MESSAGE_I_H
#include "Event.h"
#include "Timer.h"
#include "./Interactable.h"

class SimpleMessageI : public Interactable {
    public:
        SimpleMessageI(Point &pP, string &tN, string message, CollidableData cd);
        ~SimpleMessageI();
};

#endif
