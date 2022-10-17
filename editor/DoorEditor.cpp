#include "editor/DoorEditor.h"

DoorEditor::DoorEditor(Point p) : state(DoorEditState::closedSprite), input("") {
    spriteEditor = nullptr;
    rayEditor = nullptr;
    text = new Text(Point(LETTER_WIDTH * 2, LETTER_HEIGHT * 4), "");
    UIRenderer::addText(text);
    updateText();
    door = new Door(p);
    gameState = GameState::TextInput;
}

DoorEditor::~DoorEditor() {
    UIRenderer::removeText(text);
}



int DoorEditor::routeInput(KeyPresses keysDown) {
    switch (state) {
        case DoorEditState::closedSprite:
            if (!spriteEditor)
                handleTextInput(keysDown);
            else if (spriteEditor->routeInput(keysDown)) {
                delete spriteEditor;
                spriteEditor = nullptr;
                door->calculateHeight();
                state = DoorEditState::openSprite;
                gameState = GameState::TextInput;
                updateText();
            }
            break;
        case DoorEditState::openSprite:
            if (!spriteEditor) {
                handleTextInput(keysDown);
            }
            else if (spriteEditor->routeInput(keysDown)) {
                delete spriteEditor;
                spriteEditor = nullptr;
                door->calculateHeight();
                state = DoorEditState::move;
            }
            break;
        case DoorEditState::move:
            door->manuallyControl(keysDown);
            if (keysDown.ok) {
                rayEditor = new RayEditor(door);
                state = DoorEditState::drawLines;
                rayEditor->type = CollidableType::obstruction;
                rayEditor->name = "obstruction";
                rayEditor->layer = 0;
                rayEditor->editState = RayEditState::move;
            }
            break;
        case DoorEditState::drawLines:
            if (door->obstructions.size() > 0 || rayEditor->routeInput(keysDown)) {
                Ray *r = new Ray(*rayEditor->ray);
                door->addInteractable("interactable")->addRay(r);
                door->addTrigger("triggers")->addRay(r);
                door->close();
                delete rayEditor;
                rayEditor = nullptr;
                return 1;
            }
        break;
    }
    return 0;
}

void DoorEditor::updateText() {
    string displayText;
    string spriteType = state == DoorEditState::closedSprite ? "closed" : "open";
    switch (state) {
        case DoorEditState::closedSprite:
        case DoorEditState::openSprite:
            if (!spriteEditor)
                displayText = "Enter " + spriteType + " sprite path:` " + input;
            else 
                displayText = "Edit " + spriteType + " sprite";
            break;
        case DoorEditState::move:
            displayText = "Move door";
        case DoorEditState::drawLines:

            break;
    }
    text->setText(displayText);
}


void DoorEditor::handleTextInput(KeyPresses keysDown) {
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        updateText();
    }
    if (keysDown.del && input.length() > 0) {
        input.pop_back();
        updateText();
    }
    if (keysDown.start && SpriteEditor::checkPath(input)) {
        spriteEditor = new SpriteEditor(door->AddRawSprite("./assets/" + input));
        Sprite* s = spriteEditor->sprite;
        door->highlightSprite(s);
        s->frontAndCenter();
        gameState = GameState::FieldFree;
        input.clear();
        text->clearText();
    }
}