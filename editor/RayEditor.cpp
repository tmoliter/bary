#include "RayEditor.h"

RayEditor::RayEditor(RealThing *p) : 
    parent(p),
    layer(0),
    editState(RayEditState::selectType), 
    cameraPrevState(RayEditState::selectType),
    type(CollidableType::obstruction),
    name("") {
    
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
    if (type == CollidableType::obstruction)
        parent->addObstruction({ new Ray(ray->a,ray->b) }, layer);
    if (type == CollidableType::interactable) {
        bool editExisting = parent->interactables.count(name);
        Interactable *in = parent->addInteractable(name);
        if (!editExisting)
            in->layer = layer;
        in->addRay(new Ray(ray->a,ray->b));
    }
    updateLines();
    editState = RayEditState::move;
}

int RayEditor::nextMode() {
    switch (editState) {
    case RayEditState::selectType:
        if (type == CollidableType::obstruction)
            updateLines();
            editState = RayEditState::layer;
        if (type == CollidableType::interactable) {
            gameState = GameState::TextInput;
            editState = RayEditState::name;
        }
        return 0;
    case RayEditState::name:
        handleNameSubmit();
        return 0;
    case RayEditState::layer:
        editState = RayEditState::move;
        return 0;
    case RayEditState::move:
        editState = RayEditState::stretch;
        return 0;
    case RayEditState::stretch:
        saveRay();
        return 0;
    default:
        return 0;
    }
}

int RayEditor::lastMode() {
    switch (editState) {
    case RayEditState::selectType:
        return 1;
    case RayEditState::name:
        return 0;
    case RayEditState::layer:
        if (type == CollidableType::obstruction)
            return 1;
        if (type == CollidableType::interactable)
            editState = RayEditState::name;
        return 0;
    case RayEditState::move:
        return 1;
    case RayEditState::stretch:
        editState = RayEditState::move;
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
    string prefix = name.size() > 0 ? name + ": " : "";
    string suffix = "`layer: " + to_string(layer);
    switch (editState) {
    case RayEditState::selectType:
        displayText = type == CollidableType::interactable ? "interactable" : "collidable";
        break;
    case RayEditState::name:
        displayText = "enter name: " + name;
        if (parent->interactables.size() > 0) {
            displayText = displayText + "``existing interactables:";
            for (auto const& [name, in] : parent->interactables)
                displayText = displayText + "`  " + name;
        }
        break;
    case RayEditState::layer:
        displayText = prefix + "set layer" + suffix;
        break;
    case RayEditState::move:
        displayText = prefix + "move" + suffix;
        break;
    case RayEditState::stretch:
        displayText = prefix + "stretch" + suffix;
        break;
    case RayEditState::cameraMove:
        displayText = prefix + "camera move" + suffix;
        break;
    }
    displayText = displayText + "`" + ray->getStringPosition();
    text->setText(displayText);
}

void RayEditor::updateLines() {
    parent->showObstructionLines(layer);
    parent->showInteractableLines(layer, name);
}

void RayEditor::handleNameSubmit() {
    if (name.size() < 1)
        return;
    if (parent->interactables.count(name)) {
        editState = RayEditState::move;
        layer =  parent->interactables[name]->layer;
        updateLines();
    }
    else
        editState = RayEditState::layer;
    gameState = GameState::FieldFree;
}

int RayEditor::routeInput(KeyPresses keysDown) {
    if(keysDown.ok)
        return nextMode();
    if(keysDown.cancel)
        return lastMode();
    handleCameraControls(keysDown);
    displayText();
    switch (editState) {
    case RayEditState::selectType:
        setType(keysDown);
        break;
    case RayEditState::name:
        handleNameInput(keysDown);
        break;
    case RayEditState::layer:
        editLayer(keysDown);
        break;
    case RayEditState::move:
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

void RayEditor::setType (KeyPresses keysDown) {
    if( keysDown.up || keysDown.debug_up)
        type = CollidableType::obstruction;
    if (keysDown.down || keysDown.debug_down)
        type = CollidableType::interactable;
}

void RayEditor::handleNameInput(KeyPresses keysDown) {
    if (keysDown.textInput)
        name.push_back(keysDown.textInput);
    if (keysDown.del && name.length() > 0)
        name.pop_back();
    if (keysDown.start) {
        nextMode();
    }
}

void RayEditor::editLayer (KeyPresses keysDown) {
    if(keysDown.up || keysDown.debug_up) {
        layer += 1;
        updateLines();
    }
    if(keysDown.down || keysDown.debug_down) {
        layer -= 1;
        updateLines();
    }
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
