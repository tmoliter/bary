#include "./burgEvents.h"

Event* burgEvents::testEvent() {
    Phrase *ph = new Phrase(Point(100,100), Point(300, 70), ScrollType::continuous, "What's that??``Is something up and to my right? I'd better go check this shit out.`I say again..``What's that??``Let's see...");
    Phrase *ph2 = new Phrase(Point(150,150), Point(400, 24), ScrollType::allButLast, "Damn, nothing.", Point(1000,1000), 2);

    Event *event = new Event();
    EventNode *node1 = event->addNode(new EventNode(
        nullptr, 
        ph, 
        &testActions::test_event_node_callback, 
        &testActions::test_event_node_callback
    ));
    event->addChild(node1, new EventNode(
        nullptr, 
        ph2, 
        &testActions::test_event_node_callback, 
        &testActions::test_standlone_message_callback
    ));

    return event;
}