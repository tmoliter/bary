#include "events/SimpleMessage.h"

SimpleMessage::SimpleMessage(Phrase* ph, string soundPath) : Event() {
    if (ph)
        addPhrase(ph);
}

void SimpleMessage::addPhrase(Phrase* ph, string soundPath) {
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
    if (soundPath != "")
        nodes.back()->addSound(soundPath);
}