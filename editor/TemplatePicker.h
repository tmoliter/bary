#ifndef TEMPLATE_PICKER_H
#define TEMPLATE_PICKER_H
#include "editor/DoorEditor.h"

using namespace std;

enum class Template {
    none,
    door
};

struct TemplatePicker {
    TemplatePicker(Point p);
    // TemplatePicker(Door *door);
    // ~TemplatePicker();

    Point newThingPosition;

    Template templ;
    string input;
    Text *text;

    DoorEditor *doorEditor;

    int chooseTemplate(KeyPresses keysDown);
};

#endif