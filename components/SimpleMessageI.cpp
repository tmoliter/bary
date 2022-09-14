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

int test_standlone_message_callback() {
        Phrase *ph = new Phrase(Point(300,200), 150, 40, 1, ScrollType::continuous, "Another scrolling phrase that will just continue on for a while and then eventually it will destroy itself", 3, true);
        UIRenderer::addPhrase(ph);
        return 1;
    };


SimpleMessageI::SimpleMessageI(Thing* parent, string message, CollidableData cd) : Interactable(parent, cd) {
    Phrase *ph = new Phrase(Point(100,100), 150, 40, 1, ScrollType::continuous, "What's that??`Is something up and to my right? I'd better go check this shit out.", 2);
    Phrase *ph2 = new Phrase(Point(200,150), 300, 24, 2, ScrollType::allButLast, "Damn, nothing.");
    EventNode *node = new EventNode(&node2, ph, &test_event_node_callback, &test_event_node_callback);
    node2 = new EventNode(NULL, ph2, &test_event_node_callback, &test_standlone_message_callback);
    event = new Event();
    event->addNode(node);
    event->addNode(node2);
}

SimpleMessageI::~SimpleMessageI() {
    interactables.erase(id);
}
