// top level functions
inline begin_event()
inline terminate_event()


EventNode {
    bool terminal
    phrase *phrase
    fn *enterAction
    fn *exitAction
    fn *nextNode
    // destructor takes care of killing functions, unless they are inline (callback != &open_ui)

}

SimpleMessageNode : EventNode {
    // constructor just takes a message and a next node and we automagically spin up the empty fuctions
}


ReceiveItemEvent : EventNode {

}

Event {
    vector *EventNode nodes;
    // destructor destroys all nodes as well
}

SimpleMessageEvent : Event {
    // Just has one SimpleMessageEventNode
}

ReceiveItemEvent : Event {
    // Just has one ReceiveItemEventNode
}
 
// etc

// Eventnode lifecycle:
// - enterAction is called
// - phrase is added to UIManager
// - listen for input and advance phrase (if one exists) 
// - poll until phrase is complete, at which time:
// --- phrase is removed from UI manager
// --- exitAction is called
// --- nextNode is called to  queue up the next node. 
// --- If nextNode returns null,

// This happens in the Event class