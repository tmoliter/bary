#include "editor/TemplatePicker.h"

TemplatePicker::TemplatePicker(Point p) : newThingPosition(p), templ(Template::none) {
    doorEditor = new DoorEditor(p);
}

int TemplatePicker::chooseTemplate(KeyPresses keysDown) {
    if (doorEditor->routeInput(keysDown)) {
        delete doorEditor;
        return 1;
    }
    return 0;
}
