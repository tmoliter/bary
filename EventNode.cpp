#include "EventNode.h"

EventNode::EventNode(int (*ent)(void), int (*ex)(void), int (*next)(EventNode*&), Phrase *ph) {
    enterAction = ent;
    exitAction = ex;
    nextNode = next;
    phrase = ph;
}

EventNode::~EventNode() {
    delete phrase;
}

void EventNode::loadPhrase() {
    if(phrase != nullptr)
        UIRenderer::addPhrase(phrase);
}

int EventNode::hold (KeyPresses keysDown) {
    if(phrase->isComplete()) {
        UIRenderer::removePhrase(phrase);
        return 1;
    }
    if (keysDown.ok)
        phrase->advance();
    return 0;
}