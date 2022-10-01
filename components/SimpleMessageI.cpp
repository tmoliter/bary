#include "./SimpleMessageI.h"

SimpleMessageI::SimpleMessageI(Point &pP, string &tN,string message, CollidableData cd) : Interactable(pP, tN, cd) {
    Phrase *ph = new Phrase(Point(100,100), 300, 70, 1, ScrollType::continuous, message, 2);
    EventNode *node = new EventNode(nullptr, ph);
    event->addNode(node);
}

SimpleMessageI::~SimpleMessageI() {
    interactables.erase(id);
}
