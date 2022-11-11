#include "editor/TemplatePicker.h"

TemplatePicker::TemplatePicker(Point p) : newThingPosition(p), templ(Template::none) {
    doorEditor = new DoorEditor(p);
}

TemplatePicker::TemplatePicker(Door *door) : newThingPosition(door->position), templ(Template::door) {
    doorEditor = new DoorEditor(door);
}

TemplatePicker::TemplatePicker(RealThing *rt) : newThingPosition(rt->position) {
    Door *door = dynamic_cast<Door*>(rt);
    if (door) {
        doorEditor = new DoorEditor(door);
        templ = Template::door;
        return;
    }
    templ = Template::none;
}

int TemplatePicker::chooseTemplate(KeyPresses keysDown) {
    if (doorEditor->routeInput(keysDown)) {
        delete doorEditor;
        return 1;
    }
    return 0;
}
