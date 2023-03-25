#include "ThingEditor.h"

ThingEditor::ThingEditor() {
    thing = new RealThing(Point(0,0));

    SpriteData sd;
    sd.path = "./assets/debug/9x9cross.png";
    sd.layer = 100;
    cross = new Sprite(thing->position, thing->name, sd);
    cross->centerOffset();

    RealThing::hideAllLines();
    thing->showLines();
}

ThingEditor::~ThingEditor() {
    if (thing->sprites.size() < 1)
        delete thing;
    delete cross;
    RealThing::showAllLines();
}

void ThingEditor::changeState(ThingEditState newState) {
    switch (newState) {
        case ThingEditState::commandInput:
            commandLine = new CommandLine({
                "sprite",
                "edit sprite", 
                "ray",
                "event",
                "free"
            });
        default:
            return;
    }
}

void ThingEditor::meat(KeyPresses keysDown) {

}