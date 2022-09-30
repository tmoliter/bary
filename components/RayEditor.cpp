#include "RayEditor.h"

// We should add choosing layer as an mode, and then when we save,
// Look for an obstruction with that layer in the parent.
// If none exists, we create a new obstruction, otherwise use existing.

// Also TODO: display relative coordinates as text.

// Also: https://stackoverflow.com/questions/11363737/enum-value-collision-with-enum-name

RayEditor::RayEditor(RealThing *p) : 
    parent(p), 
    editState(RayEditState::rMove), 
    cameraPrevState(RayEditState::rMove) {
    
    ray = new Ray(Point(0,0), Point(0,0));
    UIRenderer::addLine(parent->position.x, parent->position.y, ray, editing);
    
    oldFocus = Thing::things[Camera::getFocusName()];
    focus = new Thing(Point(parent->position.x, parent->position.y));
    Camera::panTo(focus->name);

    text = new Text(Point(parent->position.x, parent->position.y), "");
    UIRenderer::addText(text);
};

RayEditor::~RayEditor() {
    UIRenderer::removeLine(ray);
    UIRenderer::removeText(text);
    Camera::panTo(oldFocus->name);
    delete focus;
};

int RayEditor::nextMode() {
    switch (editState) {
    case RayEditState::rMove:
        editState = RayEditState::stretch;
        return 0;
    case RayEditState::stretch:
        return 1;
    default:
        return 0;
    }
}

int RayEditor::lastMode() {
    switch (editState) {
    case RayEditState::rMove:
        return 0;
    case RayEditState::stretch:
        editState = RayEditState::rMove;
        return 0;
    default:
        return 0;
    }
}

void RayEditor::handleCameraControls(KeyPresses keysDown) {
    if (editState == RayEditState::rCameraMove) {
        if (keysDown.menu2)
            editState = cameraPrevState;
        focus->manuallyControl(keysDown);
        return;
    }
    if (keysDown.menu2) {
        cameraPrevState = editState;
        editState = RayEditState::rCameraMove;
    }
}

void RayEditor::displayText() {
    text->setPos(Point(LETTER_WIDTH * 4, LETTER_HEIGHT * 2));

    string displayText;
    switch (editState) {
    case RayEditState::rMove:
        displayText = "move";
        break;
    case RayEditState::stretch:
        displayText = "stretch";
        break;
    case RayEditState::rCameraMove:
        displayText = "camera move";
        break;
    }
    text->setText(displayText);
}

int RayEditor::routeInput(KeyPresses keysDown) {
    if(keysDown.ok)
        return nextMode();
    if(keysDown.cancel)
        return lastMode();
    handleCameraControls(keysDown);
    displayText();
    switch (editState) {
    case RayEditState::rMove:
        move(keysDown);
        break;
    case RayEditState::stretch:
        stretch(keysDown);
        break;
    default:
        break;
    }
    return 0;
}

void RayEditor::move (KeyPresses keysDown) {
    if((keysDown.up || keysDown.debug_up)) {
        ray->a.y -= 1;
        ray->b.y -= 1;
    }
    if((keysDown.down || keysDown.debug_down)) {
        ray->a.y += 1;
        ray->b.y += 1;
    }
    if((keysDown.left || keysDown.debug_left)) {
        ray->a.x -= 1;
        ray->b.x -= 1;
    }
    if((keysDown.right || keysDown.debug_right)) {
        ray->a.x += 1;
        ray->b.x += 1;
    }
}

void RayEditor::stretch (KeyPresses keysDown) {
    if(keysDown.up || keysDown.debug_up) {
        ray->b.y -= 1;
    }
    if(keysDown.down || keysDown.debug_down) {
        ray->b.y += 1;
    }
    if(keysDown.left || keysDown.debug_left) {
        ray->b.x -= 1;
    }
    if(keysDown.right || keysDown.debug_right) {
        ray->b.x += 1;
    }
}
