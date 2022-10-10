#include "events/SimpleMessage.h"

SimpleMessage::SimpleMessage(Phrase* ph) : Event() {
    if (ph)
        addPhrase(ph);
}

void SimpleMessage::addPhrase(Phrase* ph) {
    if (nodes.size() > 0) {
        addChild(nodes.back(), new EventNode(
        nullptr, 
        ph
    ));
    } else {
        addNode(new EventNode(
        nullptr, 
        ph
        ));
    }
}