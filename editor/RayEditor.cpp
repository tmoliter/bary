#include "RayEditor.h"

RayEditor::RayEditor(RealThing *p) : 
    parent(p),
    layer(0),
    editState(RayEditState::move), 
    cameraPrevState(RayEditState::move) {
    
    ray = new Ray(Point(0,0), Point(0,0));
    UIRenderer::addLine(parent->position.x, parent->position.y, ray, LineType::editing);
    
    oldFocus = Thing::things[Camera::getFocusName()];
    focus = new Thing(Point(parent->position.x, parent->position.y));
    Camera::panTo(focus->name);

    text = new Text(Point(LETTER_WIDTH * 2, LETTER_HEIGHT * 4), "");
    UIRenderer::addText(text);
};

RayEditor::~RayEditor() {
    UIRenderer::removeText(text);
    UIRenderer::removeLine(ray);
    Camera::panTo(oldFocus->name);
    delete ray;
    delete focus;
};

void RayEditor::saveRay() {
    Obstruction* obstruction = nullptr;
    for (auto o : parent->obstructions) {
        if (o->layer == layer) {
            obstruction = o;
            break;
        }
    }
    if (!obstruction){
        obstruction = new Obstruction(parent);
        parent->obstructions.push_back(obstruction);
    }
    Ray *r = new Ray(ray->a,ray->b);
    obstruction->addRay(r);
    editState = RayEditState::move;
}

int RayEditor::nextMode() {
    switch (editState) {
    case RayEditState::move:
        editState = RayEditState::stretch;
        return 0;
    case RayEditState::stretch:
        editState = RayEditState::layer;
        return 0;
    case RayEditState::layer:
        saveRay();
        return 0;
    default:
        return 0;
    }
}

int RayEditor::lastMode() {
    switch (editState) {
    case RayEditState::move:
        return 1;
    case RayEditState::stretch:
        editState = RayEditState::move;
        return 0;
    case RayEditState::layer:
        editState = RayEditState::stretch;
        return 0;
    default:
        return 0;
    }
}

void RayEditor::handleCameraControls(KeyPresses keysDown) {
    if (editState == RayEditState::cameraMove) {
        if (keysDown.menu2)
            editState = cameraPrevState;
        focus->manuallyControl(keysDown);
        return;
    }
    if (keysDown.menu2) {
        cameraPrevState = editState;
        editState = RayEditState::cameraMove;
    }
}

void RayEditor::displayText() {
    string displayText;
    switch (editState) {
    case RayEditState::move:
        displayText = "move";
        break;
    case RayEditState::stretch:
        displayText = "stretch";
        break;
    case RayEditState::layer:
        displayText = "layer: " + to_string(layer);
        break;
    case RayEditState::cameraMove:
        displayText = "camera move";
        break;
    }
    displayText = displayText + "`" + ray->getStringPosition();
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
    case RayEditState::move:
        move(keysDown);
        break;
    case RayEditState::stretch:
        stretch(keysDown);
        break;
    case RayEditState::layer:
        editLayer(keysDown);
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

void RayEditor::editLayer (KeyPresses keysDown) {
    if(keysDown.up || keysDown.debug_up)
        layer += 1;
    if(keysDown.down || keysDown.debug_down)
        layer -= 1;
}
