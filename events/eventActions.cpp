#include "eventActions.h"

int testActions::test_event_node_callback () {
    Timer::startOrIgnore("test");
    DirectionMap dM;
    int time = Timer::timeSince("test");
    if(time < 60) {
        if (time > 30)
            dM.right = true;
        dM.down = true;
        FieldPlayer::player->walk->move(dM);
        return 0;
    }
    Timer::destroy("test");
    return 1;
};

int testActions::test_standlone_message_callback() {
    Phrase *ph = new Phrase(Point(300,200), 150, 40, 1, ScrollType::continuous, "Another scrolling phrase that will just continue on for a while and then eventually it will destroy itself", 3, true);
    UIRenderer::addPhrase(ph);
    return 1;
};