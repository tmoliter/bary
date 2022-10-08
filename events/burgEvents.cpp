#include "./burgEvents.h"

Event* burgEvents::testEvent() {
    Phrase *ph = new Phrase(Point(100,100), 300, 70, 1, ScrollType::continuous, "What's that??``Is something up and to my right? I'd better go check this shit out.`I say again..``What's that??``Let's see...", 2);
    Phrase *ph2 = new Phrase(Point(200,150), 300, 24, 2, ScrollType::allButLast, "Damn, nothing.");

    EventNode *node;
    EventNode **node2pointer = new EventNode*;
    *node2pointer = new EventNode(nullptr, ph2, &testActions::test_event_node_callback, &testActions::test_standlone_message_callback);
    node = new EventNode(node2pointer, ph, &testActions::test_event_node_callback, &testActions::test_event_node_callback);

    Event *event = new Event();
    event->addNode(node);
    event->addNode(*node2pointer);

    return event;

    // RealThing* ss = RealThing::findRealThing(thingName);
    // ss->addTrigger(interactableName,{},0, event);
    // ss->triggers["empty door message"]->addRay(new Ray(117,252,173,252));
}