#include "editor/DoorEditor.h"

DoorEditor::DoorEditor(Point p) : state(DoorEditState::closedSprite), input("") {
    spriteEditor = nullptr;
    rayEditor = nullptr;
    text = new Text(Point(settings.LETTER_WIDTH * 2, settings.LETTER_HEIGHT * 2), "");
    UIRenderer::addText(text);
    door = static_cast<Door*>(Scene::currentScene->addThingToScene(new Door(p)));
    changeState(DoorEditState::closedSprite);
}

DoorEditor::DoorEditor(Door *door) : door(door), state(DoorEditState::closedSprite), input("") {
    spriteEditor = nullptr;
    rayEditor = nullptr;
    text = new Text(Point(settings.LETTER_WIDTH * 2, settings.LETTER_HEIGHT * 2), "");
    UIRenderer::addText(text);
    door->preview();
    changeState(DoorEditState::closedSprite);
}


DoorEditor::~DoorEditor() {
    UIRenderer::removeText(text);
    delete spriteEditor;
    delete rayEditor;
}

void DoorEditor::changeState(DoorEditState newState) {
    switch (newState) {
        case DoorEditState::closedSprite:
            if (door->sprites.size() > 0) {
                gameState = GameState::FieldFree;
                Sprite* s = door->sprites[0];
                spriteEditor = new SpriteEditor(s);
                door->highlightSprite(s);
                spriteEditor->snapCamera = true;
            } else
                gameState = GameState::TextInput;
            break;
        case DoorEditState::openSprite:
            Sprite *s;
            if (door->sprites.size() > 1) {
                s = door->sprites[1];
            } else if (door->sprites.size() > 0) {
                s = door->AddSprite(door->sprites[0]->d);
            } else {
                gameState = GameState::TextInput;
                break;
            }
            spriteEditor = new SpriteEditor(s);
            spriteEditor->snapCamera = true;
            door->highlightSprite(s);
            gameState = GameState::FieldFree;
            break;
        case DoorEditState::move:
            door->removeHighlight();
            gameState = GameState::FieldFree;
            break;
        case DoorEditState::drawLines:
            rayEditor = new RayEditor(door);
            if (door->obstructions.size() > 0) {
                rayEditor->ray->a = door->obstructions.begin()->second->rays[0]->a;
                rayEditor->ray->b = door->obstructions.begin()->second->rays[0]->b;
                door->removeAllCollidables();
            }
            state = DoorEditState::drawLines;
            rayEditor->type = CollidableType::obstruction;
            rayEditor->name = "obstruction";
            rayEditor->layer = door->sprites[0]->d.layer;
            rayEditor->editState = RayEditState::move;
            rayEditor->displayText();
            gameState = GameState::FieldFree;
        break;
    }
    state = newState;
    updateText();
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
                changeState(DoorEditState::openSprite);
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
                changeState(DoorEditState::move);
            }
            break;
        case DoorEditState::move:
            door->manuallyControl(keysDown);
            if (keysDown.ok) 
                changeState(DoorEditState::drawLines);
            if (keysDown.cancel)
                changeState(DoorEditState::closedSprite);
            break;
        case DoorEditState::drawLines:
            if (door->obstructions.size() > 0 || rayEditor->routeInput(keysDown)) {
                Ray *r;
                r = new Ray(*rayEditor->ray);
                door->addInteractable("interactable")->addRay(r);
                r = new Ray(*rayEditor->ray);
                door->addTrigger("trigger")->addRay(r);
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
                displayText = "Enter sprite path:` " + input;
            else 
                displayText = "Edit " + spriteType + " sprite";
            break;
        case DoorEditState::move:
            displayText = "Move door";
            break;
        case DoorEditState::drawLines:
            displayText = "Draw collidable ray";
            break;
    }
    text->setText(displayText);
}


void DoorEditor::handleTextInput(KeyPresses keysDown) {
    // When we do the next "template" type thing, we should encapsulate this logic and consolidate it for
    // ThingEditor, DoorEditor, and future editors. We should make use of CommandLine
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        updateText();
    }
    if (keysDown.del && input.length() > 0) {
        input.pop_back();
        updateText();
    }
    if (keysDown.start) {
        spriteEditor = new SpriteEditor(door->AddRawSprite(input));
        spriteEditor->snapCamera = true;
        Sprite* s = spriteEditor->sprite;
        door->highlightSprite(s);
        s->frontAndCenter();
        gameState = GameState::FieldFree;
        input.clear();
        updateText();
    }
}