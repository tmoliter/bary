#include "Event.h"

Event::Event() {
    activeEvents.push_back(this);
    stage = EventStage::enterNode;
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
    current = nodes.front();
}

void Event::executeEnter() {
    if(current->enterAction())
        stage = EventStage::holdNode;
}

void Event::holdNode(KeyPresses keysDown) {
    if(current->hold(keysDown)) 
        stage = EventStage::exitNode;
}

void Event::endNode() {
    if(current->exitAction()) {
        EventNode *tmp = current->nextNode();
        if (!tmp) {
            stage = EventStage::terminateEvent;
            return;
        }
        current = tmp;
        stage = EventStage::enterNode;
    }
};

void Event::meat(KeyPresses keysDown) {
    if (activeEvents.size() < 1)
        return;

    Event *event = activeEvents.front();
    switch (event->stage) {
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
            delete event;

    }
}