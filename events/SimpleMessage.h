#ifndef SIMPLE_MESSAGE_H
#define SIMPLE_MESSAGE_H
#include "events/Event.h"
#include <string>
#include <vector>

struct SimpleMessage : public Event {
    SimpleMessage(Phrase *ph = nullptr, string soundName = "");
    void addPhrase(Phrase *ph, string soundName = "");
};

#endif