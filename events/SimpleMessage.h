#ifndef SIMPLE_MESSAGE_H
#define SIMPLE_MESSAGE_H
#include "events/Event.h"
#include <string>

struct SimpleMessage : public Event {
    SimpleMessage() : Event() {};
    void addMessage(string newMessage);
};

#endif