#include "EventNode.h"

EventNode::EventNode(EventNode **nn, Phrase *ph, int (*ent)(void), int (*ex)(void), int (*nextCB)(EventNode*&)) :
    nextNode(nn),
    phrase(ph),
    enterAction(ent),
    exitAction(ex),
    nextNodeCB(nextCB) {};

EventNode::~EventNode() {
    delete *nextNode;
    delete nextNode;
    delete phrase;
}

void EventNode::loadPhrase() {
    if(phrase != nullptr) {
        phrase-> autoDestroy = false;
        UIRenderer::addPhrase(phrase);
    }
}

int EventNode::hold (KeyPresses keysDown) {
    if(phrase == nullptr) 
        return 1;
    if(phrase->isComplete() && !phrase->autoDestroy) {
        phrase->reset();
        UIRenderer::removePhrase(phrase);
        return 1;
    }
    if (keysDown.ok)
        phrase->advance();
    return 0;
}

int EventNode::getNextNode(EventNode *&node) {
    if (nextNode != nullptr && *nextNode != nullptr){
        node = *nextNode;
        return 1;
    }
    if (nextNodeCB != nullptr){
        nextNodeCB(node);
        return 1;
    }
    return 0;
}
