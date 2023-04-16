#include "events/SimpleMessage.h"

SimpleMessage::SimpleMessage(Phrase* ph, string soundName) : Event() {
    if (ph)
        addPhrase(ph, soundName);
}

void SimpleMessage::addPhrase(Phrase* ph, string soundName) {
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
    if (soundName != "")
        nodes.back()->addSound(soundName);
}