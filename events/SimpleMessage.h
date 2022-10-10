#ifndef SIMPLE_MESSAGE_H
#define SIMPLE_MESSAGE_H
#include "events/Event.h"
#include <string>
#include <vector>

struct SimpleMessage : public Event {
    SimpleMessage() : Event() {};
    void addPhrase(Phrase *ph);
};

#endif