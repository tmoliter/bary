#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : input("") {
    mapBuilder = this;
    commandText = nullptr;
    cross = nullptr;

    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
    
    currentThing = dotThing = new RealThing(Point(0,0), "EditorDot");
    SpriteData dotSD;
    dotSD.path = "assets/debug/onePixel.png";
    dotThing->AddSprite(new Sprite(
        dotThing->position.x, 
        dotThing->position.y, 
        dotThing->name, 
        dotSD
    ));
    changeState(EditorState::freeMove);
}

void MapBuilder::changeState(EditorState newState) {
    string prefix = currentThing != dotThing ? currentThing->name + ": " : "";
    switch (newState) {
        case freeMove:
            helpText->setText("Free Move");
            endTextInput();
            focusDot();
            Sprite::removeHighlight();
            state = EditorState::freeMove;
            break;
        case thingMove:
            helpText->setText(prefix + "Thing Move");
            endTextInput();
            Camera::panTo(currentThing->name);
            state = EditorState::thingMove;
            break;
        case commandInput:
            helpText->setText(prefix + "Enter Command");
            beginTextInput();
            if (currentThing != dotThing)
                currentThing->removeHighlight();
            state = EditorState::commandInput;
            break;
        case renameThing:
            helpText->setText(prefix + ": Enter New Name");
            beginTextInput();
            Sprite::highlightThing(currentThing->name);
            state = EditorState::renameThing;
            break;
        case pathInput:
            helpText->setText(prefix + "Enter Sprite Path");
            beginTextInput();
            if (currentThing != dotThing)
                currentThing->highlightSprite(nullptr);
            state = EditorState::pathInput;
            break;
        case spriteEdit:
            helpText->setText(prefix + ": Sprite Edit Mode");
            endTextInput();
            state = EditorState::spriteEdit;
            break;
    }
}


void MapBuilder::createOrSelectThing() {
    if (currentThing != dotThing)
        return;
    currentThing = new RealThing(Point(currentThing->position.x, currentThing->position.y));
    SpriteData sd;
    sd.path = "./assets/debug/9x9cross.png";
    sd.layer = 100;
    cross = new Sprite(currentThing->position.x, currentThing->position.y, currentThing->name, sd);
    cross->centerOffset();
}

void MapBuilder::focusDot() {
    currentThing = dotThing;
    Camera::panTo(currentThing->name);
    if (cross == nullptr)
        return;
    delete cross;
    cross = nullptr;
}


void MapBuilder::beginTextInput() {
    gameState = GameState::TextInput;
    if (commandText == nullptr) {
        commandText = new Text(Point(32, 32), "");
        UIRenderer::addText(commandText);
    }
    input.clear();
    commandText->clearText();
}

void MapBuilder::endTextInput() {
    gameState = GameState::FieldFree;
    input.clear();
    if (commandText == nullptr)
        return;
    commandText->clearText();
    UIRenderer::removeText(commandText);
    commandText = nullptr;
}


int MapBuilder::listenForTextInput(KeyPresses keysDown) {
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        commandText->setText(input);
        return 1;
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        commandText->setText(input);
        return 1;
    }
    return 0;
}


void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (state == EditorState::freeMove || state == EditorState::thingMove) {
        currentThing->manuallyControl(keysDown);
        if(keysDown.start) {
            changeState(EditorState::commandInput);
            return;
        }
    }

    if (state == EditorState::freeMove) {
        if (keysDown.ok) {
            vector<Thing*> collisions = Thing::findThingsByPoint(dotThing->position);
            for (auto t : collisions) {
                if (t != dotThing) {
                    RealThing* match = dynamic_cast<RealThing*>(t);
                    if (match) {
                        currentThing = match;
                        changeState(EditorState::commandInput);
                        return;
                    }
                }
            }
        }
    }


    if (state == EditorState::commandInput) {
        if (listenForTextInput(keysDown))
            return;
        if (keysDown.start) {
            if (input == "sprite") {
                changeState(EditorState::pathInput);
                return;
            }
            if (input == "free") {
                changeState(EditorState::freeMove);
                return;
            }
            if (currentThing != dotThing) {
                if (input == "rename") {
                    changeState(EditorState::renameThing);
                    return;
                }
                if (input == "move") {
                    changeState(EditorState::thingMove);
                    return;
                }
            }
            changeState(EditorState::commandInput);
        }
    }

    if (state == EditorState::pathInput) {
        if (listenForTextInput(keysDown))
            return;
        if(keysDown.start) {
            if(addSprite())
                changeState(EditorState::spriteEdit);
            else
                changeState(EditorState::commandInput);
        }
    }

    if (state == EditorState::renameThing) {
        if (listenForTextInput(keysDown))
            return;
        if(keysDown.start) {
            if (input.length() > 0)
                currentThing->rename(input);
            changeState(EditorState::commandInput);
        }
    }

    if(state == EditorState::spriteEdit) {
        if(spriteEditor->routeInput(keysDown)) {
            delete spriteEditor;
            currentThing->calculateHeight();
            changeState(EditorState::commandInput);
        }
    }
}

int MapBuilder::addSprite() {
    string possiblePath = string(BASE_PATH) + "assets/" + input;
    if(filesystem::path(possiblePath).extension() != ".png")
        return 0;
    const char* cPossiblePath = possiblePath.c_str();
    ifstream f(cPossiblePath);
    if(!f.good())
        return 0;
    createOrSelectThing();
    string path = "./assets/" + input;
    spriteEditor = new SpriteEditor(currentThing->AddRawSprite(path));
    spriteEditor->sprite->frontAndCenter();
    currentThing->highlightSprite(spriteEditor->sprite);
    return 1;
}