#include "RayEditor.h"

RayEditor::RayEditor(RealThing *p) : 
    parent(p),
    layer(0),
    editState(RayEditState::selectType), 
    cameraPrevState(RayEditState::selectType),
    type(CollidableType::obstruction),
    name(""),
    selected(nullptr),
    selectedI(-1) {
    
    ray = new Ray(Point(0,0), Point(0,0));
    UIRenderer::addLine(parent->position.x, parent->position.y, ray, LineType::editing);
    
    oldFocus = Thing::things[Camera::getFocusName()];
    focus = new Thing(Point(parent->position.x, parent->position.y), "ray focus");
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
    if (type == CollidableType::trigger) {
        bool editExisting = parent->triggers.count(name);
        Trigger *tr = parent->addTrigger(name);
        if (!editExisting)
            tr->layer = layer;
        tr->addRay(new Ray(ray->a,ray->b));
    }
    updateLines();
    editState = RayEditState::move;
}

int RayEditor::nextMode() {
    switch (editState) {
    case RayEditState::selectType:
        if (type == CollidableType::obstruction) {
            updateLines();
            editState = RayEditState::layer;
        } else {
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
    case RayEditState::select:
        redoRay();
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
        else
            editState = RayEditState::name;
        return 0;
    case RayEditState::move:
        return 1;
    case RayEditState::stretch:
        editState = RayEditState::move;
        return 0;
    case RayEditState::select:
        exitSelect();
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
        if (type == CollidableType::obstruction)
            displayText = "obstruction";
        if (type == CollidableType::interactable)
            displayText = "interactable";
        if (type == CollidableType::trigger)
            displayText = "trigger";
        break;
    case RayEditState::name:
        displayText = "enter name: " + name;
        if (type == CollidableType::interactable && parent->interactables.size() > 0) {
            displayText = displayText + "``existing interactables:";
            for (auto const& [name, in] : parent->interactables)
                displayText = displayText + "`  " + name;
        }
        if (type == CollidableType::trigger && parent->triggers.size() > 0) {
            displayText = displayText + "``existing triggers:";
            for (auto const& [name, tr] : parent->triggers)
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
    case RayEditState::select:
        displayText = prefix + "select to edit" + suffix;
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
    parent->showInteractableLines(layer, type == CollidableType::interactable ? name : "");
    parent->showTriggerLines(layer, type == CollidableType::trigger ? name : "");
}

void RayEditor::handleNameSubmit() {
    if (name.size() < 1)
        return;
    if (type == CollidableType::interactable && parent->interactables.count(name)) {
        editState = RayEditState::move;
        layer =  parent->interactables[name]->layer;
        updateLines();
    } else if (type == CollidableType::trigger && parent->triggers.count(name)) {
        editState = RayEditState::move;
        layer =  parent->triggers[name]->layer;
        updateLines();
    } else
        editState = RayEditState::layer;
    gameState = GameState::FieldFree;
}

void RayEditor::enterSelect () {
    editState = RayEditState::select;
    bool found = false;
    UIRenderer::changeLineType(ray, LineType::highlight);
    switch (type) {
        case CollidableType::obstruction:
            if (parent->obstructions.count(layer)) {
                selected = parent->obstructions[layer];
                found = true;
            }
            break;
        case CollidableType::interactable:
            if (parent->interactables.count(name)) {
                selected = parent->interactables[name];
                found = true;
            }
            break;
        case CollidableType::trigger:
            if (parent->triggers.count(name)) {
                selected = parent->triggers[name];
                found = true;
            }
            break;
    }
    if (!found || selected->rays.size() < 1) {
        exitSelect();
        return;
    }
    selectedI = 0;
    matchSelected();
}

void RayEditor::exitSelect() {
    UIRenderer::changeLineType(ray, LineType::editing);
    selected = nullptr;
    selectedI = -1;
    editState = RayEditState::move;
}

void RayEditor::matchSelected() {
    ray->a = selected->rays[selectedI]->a;
    ray->b = selected->rays[selectedI]->b;
    updateLines();
}

void RayEditor::redoRay() {
    selected->removeRay(selectedI);
    exitSelect();
    updateLines();
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
    case RayEditState::select:
        select(keysDown);
        break;
    default:
        break;
    }
    return 0;
}

void RayEditor::setType (KeyPresses keysDown) {
    if( keysDown.up || keysDown.debug_up) {
        switch (type) {
            case CollidableType::obstruction:
            case CollidableType::interactable:
                type = CollidableType::obstruction;
                break;
            case CollidableType::trigger:
                type = CollidableType::interactable;
                break;
        }
    }
    if (keysDown.down || keysDown.debug_down) {
        switch (type) {
            case CollidableType::obstruction:
                type = CollidableType::interactable;
                break;
            case CollidableType::interactable:
            case CollidableType::trigger:
                type = CollidableType::trigger;
                break;
        }
    }
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
    if (keysDown.menu1) {
        enterSelect();
        return;
    }
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
    if (keysDown.menu1) {
        enterSelect();
        return;
    }
    if(keysDown.up || keysDown.debug_up)
        ray->b.y -= 1;
    if(keysDown.down || keysDown.debug_down)
        ray->b.y += 1;
    if(keysDown.left || keysDown.debug_left)
        ray->b.x -= 1;
    if(keysDown.right || keysDown.debug_right)
        ray->b.x += 1;
}

void RayEditor::select (KeyPresses keysDown) {
    if (selected->rays.size() < 1) {
        exitSelect();
        return;
    }
    if(keysDown.up || keysDown.debug_up) {
        selectedI = selectedI > 0 ? (selectedI - 1) : selected->rays.size() - 1;
        matchSelected();
    }
    if(keysDown.down || keysDown.debug_down) {
        selectedI = (selectedI + 1) % selected->rays.size();
        matchSelected();
    }
}
