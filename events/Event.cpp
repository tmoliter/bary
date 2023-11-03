  #include "Event.h"

Event::Event() : references(0) {
    stage = EventStage::pending;
}

Event::~Event() {
    for (auto node : nodes)
        delete node;
}

EventNode* Event::addNode(EventNode* node) {
    nodes.push_back(node);
    return node;
}

void Event::addNodes(vector<EventNode*> newNodes) {
    for (auto node : newNodes)
        addNode(node);
}

EventNode* Event::addChild(EventNode* prev, EventNode* newNode) {
    EventNode **newNodeP = new EventNode*;
    *newNodeP = newNode;
    prev->nextNode = newNodeP;
    return addNode(newNode);
}


void Event::begin() {
    activeEvents.push_back(this);
    firstNode = nodes.front();
    current = firstNode;
    stage = EventStage::enterNode;
    // NOTE: Maybe not all nodes should change gameState
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
}

void Event::meat(KeyPresses keysDown) {
    if (activeEvents.size() < 1)
        return;

    for (auto event : activeEvents) {
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
                activeEvents.erase(remove(activeEvents.begin(), activeEvents.end(), event), activeEvents.end());
                break;
        }
    }
}

void Event::setBeginGameState() {
    gameState = GameState::FieldUI;
}

void Event::setEndGameState() {
    gameState = GameState::FieldFree;
}
