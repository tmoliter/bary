#include "events/Event.h"

Event::Event() : references(0) {
    activeEvents.push_back(this);
    stage = EventStage::pending;
}

Event::~Event() {
    activeEvents.erase(remove(activeEvents.begin(), activeEvents.end(), this), activeEvents.end());
    for (auto node : nodes) {
        delete node;
    }
}

void Event::addNode(EventNode* node) {
    nodes.push_back(node);
}

void Event::begin() {
    firstNode = nodes.front();
    current = firstNode;
    stage = EventStage::enterNode;
    setBeginGameState();
}

void Event::executeEnter() {
    if(current->enterAction == nullptr || current->enterAction()) {
        current->loadPhrase();
        stage = EventStage::holdNode;
    }
}

void Event::holdNode(KeyPresses keysDown) {
    if(current->hold(keysDown)) 
        stage = EventStage::exitNode;
}

void Event::endNode() {
    if(current->exitAction == nullptr || current->exitAction()) {
        EventNode *tmp = current;
        if (tmp->getNextNode(current)) {
            stage = EventStage::enterNode;
            return;
        }
        stage = EventStage::terminateEvent;
    }
};

void Event::meat(KeyPresses keysDown) {
    if (activeEvents.size() < 1)
        return;

    Event *event = activeEvents.front();
    switch (event->stage) {
        case (EventStage::pending):
            break;
        case (EventStage::enterNode):
            event->executeEnter();
            break;
        case (EventStage::holdNode):
            event->holdNode(keysDown);
            break;
        case (EventStage::exitNode):
            event->endNode();
            break;
        case (EventStage::terminateEvent):
            event->setEndGameState();
            event->stage = EventStage::pending;
            break;
    }
}

void Event::setBeginGameState() {
    gameState = GameState::FieldUI;
}

void Event::setEndGameState() {
    gameState = GameState::FieldFree;
}
