#include "editor/TemplatePicker.h"

TemplatePicker::TemplatePicker(Point p) : newThingPosition(p), templ(Template::none) {
    doorEditor = new DoorEditor(p);
}

TemplatePicker::TemplatePicker(Door *door) : newThingPosition(door->position), templ(Template::door) {
    doorEditor = new DoorEditor(door);
}

int TemplatePicker::chooseTemplate(KeyPresses keysDown) {
    if (doorEditor->routeInput(keysDown)) {
        delete doorEditor;
        return 1;
    }
    return 0;
}
