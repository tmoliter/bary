#ifndef FOCUS_TRACKER_H
#define FOCUS_TRACKER_H
#include "things/RealThing.h"

enum class FocusMode {
    center,
    point
};

struct FocusTracker {
    FocusTracker(RealThing* startingFocus) : focusMode(FocusMode::center) {
        setFocus(startingFocus);
        ftracker = this;
    };
    FocusMode focusMode;
    RealThing* focus;
    string getFocusName() { 
        if (focus == nullptr)
            return "";
        return focus->name;
    };
    void setFocus(RealThing* newFocus) {
        RealThing* x = newFocus;
        focus = newFocus;
        // This is kind of a hacky way to set focus mode for the map editor
        if (focus != nullptr)
            focusMode = focus->name == "EditorDot" ? FocusMode::point : FocusMode::center;
    };
    void setCameraFocalPoint() {
        if (!focus)
            return;
        Camera::c->focalPoint = focusMode == FocusMode::center ? focus->getCenter() : focus->position;
    }
    
    inline static FocusTracker *ftracker;
};

#endif
