#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : input("") {
    mapBuilder = this;

    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
    
    dotThing = new RealThing(Point(0,0));
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
    switch (newState) {
        case freeMove:
            endTextInput();
            currentThing = dotThing;
            currentThing->removeHighlight();
            state = EditorState::freeMove;
            helpText->setText("Free Move");
            break;
        case thingMove:
            endTextInput();
            state = EditorState::thingMove;
            helpText->setText("Thing Move");
            break;
        case commandInput:
            beginTextInput();
            state = EditorState::commandInput;
            helpText->setText("Enter Command");
            break;
        case pathInput:
            beginTextInput();
            state = EditorState::pathInput;
            helpText->setText("Enter Sprite Path");
            break;
        case spriteEdit:
            endTextInput();
            state = EditorState::spriteEdit;
            helpText->setText("Sprite Edit Mode");
            break;
    }
}


void MapBuilder::beginTextInput() {
    gameState = GameState::TextInput;
    if (commandText == nullptr) {
        commandText = new Text(Point(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 ), "");
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
        if (keysDown.cancel && currentThing != dotThing)
            currentThing = dotThing;
        if(keysDown.start) {
            changeState(EditorState::commandInput);
            return;
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
            changeState(EditorState::freeMove);
            endTextInput();
        }
    }

    if (state == EditorState::pathInput) {
        if (listenForTextInput(keysDown))
            return;
        if(keysDown.start) {
            if(addSprite())
                changeState(EditorState::spriteEdit);
            else
                changeState(EditorState::freeMove);
        }
    }

    if(state == EditorState::spriteEdit) {
        if(spriteEditor->routeInput(keysDown)) {
            spriteEditor->sprite->alpha = 100;
            delete spriteEditor;
            changeState(EditorState::pathInput);
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
    if (currentThing == dotThing) {
        currentThing = new RealThing(Point(currentThing->position.x, currentThing->position.y));
    }
    string path = "./assets/" + input;
    spriteEditor = new SpriteEditor(currentThing->AddRawSprite(path));
    spriteEditor->sprite->frontAndCenter();
    currentThing->highlightSprite(spriteEditor->sprite);
    return 1;
}