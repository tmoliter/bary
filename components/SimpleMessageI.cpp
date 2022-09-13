#include "./SimpleMessageI.h"

int test_event_node_callback () {
        Timer::startOrIgnore("test");
        DirectionMap dM;
        if(Timer::timeSince("test") < 60) {
            if (frameCount > 350)
                dM.right = true;
            dM.down = true;
            FieldPlayer::player->walk->move(dM);
            return 0;
        }
        Timer::destroy("test");
        return 1;
    };


SimpleMessageI::SimpleMessageI(Thing* parent, string message, CollidableData cd) : Interactable(parent, cd) {
    Phrase *ph = new Phrase(Point(200,200), 100, 40, 1, ScrollType::allButLast, "Testing an interactable.");
    EventNode *node =  new EventNode(nullptr, ph, nullptr, &test_event_node_callback);
    event = new Event();
    event->addNode(node);
}

SimpleMessageI::~SimpleMessageI() {
    interactables.erase(id);
}
