#include "EventNode.h"

EventNode::EventNode(EventNode **nn, Phrase *ph, int (*ent)(void), int (*ex)(void), int (*nextCB)(EventNode*&)) :
    nextNode(nn),
    phrase(ph),
    enterAction(ent),
    exitAction(ex),
    nextNodeCB(nextCB) {
        sound = nullptr;
    };

EventNode::~EventNode() {
    delete *nextNode;
    delete nextNode;
    delete phrase;
    resourceDepository::releaseChunk("assets/sfx/fart.mp3");
}

void EventNode::addSound(string path) {
    // if (sound != nullptr) {
        // need a wrapper around "sound" to store the path so that we can easily release
        // resourceDepository::releaseChunk()
        // sound = nullptr;
    // }
    sound = resourceDepository::getChunk(path);
}

void EventNode::loadPhrase() {
    if (sound != nullptr) {
        Mix_PlayChannel(-1, sound, 0);
    }
    if(phrase != nullptr) {
        phrase->autoDestroy = false;
        UIRenderer::addPhrase(phrase);
    }
}

int EventNode::hold (KeyPresses keysDown) {
    if(phrase == nullptr) 
        return 1;
    if(phrase->isComplete() && !phrase->autoDestroy) {
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
