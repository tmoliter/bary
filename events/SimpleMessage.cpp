#include "events/SimpleMessage.h"

void SimpleMessage::addPhrase(Phrase* ph) {
    if (nodes.size() > 1) {
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