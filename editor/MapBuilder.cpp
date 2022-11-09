#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : input(""), lastPath(""), selectedSprite(-1) {
    mapBuilder = this;
    commandText = nullptr;
    cross = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    templatePicker = nullptr;


    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);

    commandList = new Text(Point(SCREEN_WIDTH - 128, 16), "");
    UIRenderer::addText(commandList);
    
    currentThing = dotThing = new RealThing(Point(0,0), "EditorDot");
    dotThing->AddRawSprite("assets/debug/onePixel.png");
    changeState(EditorState::freeMove);
}

void MapBuilder::changeState(EditorState newState) {
    string prefix = currentThing != dotThing ? currentThing->name + ": " : "";
    switch (newState) {
        case EditorState::freeMove:
            helpText->setText("Free Move");
            endTextInput();
            focusDot();
            Sprite::removeHighlight();
            state = EditorState::freeMove;
            break;
        case EditorState::play:
            helpText->setText("Play");
            endTextInput();
            currentThing = new FieldPlayer(dotThing->position, "test player", "./assets/sheets/SDL_TestSS.png");
            Camera::panTo(currentThing->name);
            state = EditorState::play;
            break;
        case EditorState::thingMove:
            helpText->setText(prefix + "Thing Move");
            endTextInput();
            Camera::panTo(currentThing->name);
            state = EditorState::thingMove;
            break;
        case EditorState::commandInput:
            helpText->setText(prefix + "Enter Command");
            beginTextInput();
            commandList->setText("COMMANDS:` sprite` ray` free");
            if (currentThing != dotThing) {
                commandList->setText(commandList->text + "` rename` move` event` edit sprite` copy` delete");
                currentThing->removeHighlight();
            } else {
                commandList->setText(commandList->text + "` play` template");
            }
            state = EditorState::commandInput;
            break;
        case EditorState::renameThing:
            helpText->setText(prefix + "Enter New Name");
            beginTextInput();
            Sprite::highlightThing(currentThing->name);
            state = EditorState::renameThing;
            break;
        case EditorState::pathInput:
            helpText->setText(prefix + "Enter Sprite Path");
            beginTextInput();
            input = lastPath;
            commandText->setText(input);
            if (currentThing != dotThing)
                currentThing->highlightSprite(nullptr);
            state = EditorState::pathInput;
            break;
        case EditorState::spriteSelect:
            helpText->setText(prefix + "Select/Delete Sprite ");
            endTextInput();
            selectedSprite = 0;
            currentThing->highlightSprite(currentThing->sprites[selectedSprite]);
            state = EditorState::spriteSelect;
            break;
        case EditorState::spriteEdit:
            helpText->setText(prefix + "Sprite Edit Mode");
            endTextInput();
            state = EditorState::spriteEdit;
            break;
        case EditorState::rayEdit:
            createOrSelectThing();
            prefix = currentThing->name + ": ";
            helpText->setText(prefix + "Ray Edit Mode");
            endTextInput();
            rayEditor = new RayEditor(currentThing);
            state = EditorState::rayEdit;
            break;
        case EditorState::eventEdit:
            prefix = currentThing->name + ": ";
            helpText->setText(prefix + "Event Edit Mode");
            endTextInput();
            eventEditor = new EventEditor(currentThing);
            state = EditorState::eventEdit;
            break;
        case EditorState::thingFromTemplate:
            helpText->clearText();
            endTextInput();
            if (!templatePicker)
                templatePicker = new TemplatePicker(currentThing->position);
            state = EditorState::thingFromTemplate;
            break;
    }
    updateLines();
}


void MapBuilder::createOrSelectThing() {
    if (currentThing == dotThing)
        currentThing = new RealThing(Point(currentThing->position.x, currentThing->position.y));
    if (!cross) {
        SpriteData sd;
        sd.path = "./assets/debug/9x9cross.png";
        sd.layer = 100;
        cross = new Sprite(currentThing->position, currentThing->name, sd);
        cross->centerOffset();
    }
}

void MapBuilder::focusDot() {
    currentThing = dotThing;
    Camera::panTo(currentThing->name);
    if (cross == nullptr)
        return;
    delete cross;
    cross = nullptr;
}

void MapBuilder::updateLines() {
    if(currentThing == dotThing)
        RealThing::showAllLines();
    else {
        RealThing::hideAllLines();
        currentThing->showLines();
    }
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

    if(state == EditorState::play) {
        if (keysDown.start) {
            delete currentThing;
            currentThing = dotThing;
            changeState(EditorState::freeMove);
            return;
        }
    }

    if (state == EditorState::freeMove) {
        if (keysDown.ok) {
            vector<Thing*> collisions = Thing::findThingsByPoint(dotThing->position);
            for (auto t : collisions) {
                if (t != dotThing) {
                    // Here we'd always pass the thing into "TemplatePicker"
                    // and there we can decide what kind of thing it is and what to do
                    Door* door = dynamic_cast<Door*>(t);
                    if (door) {
                        templatePicker = new TemplatePicker(door);
                        changeState(EditorState::thingFromTemplate);
                        Camera::panTo(door->name);
                        return;
                    }
                    RealThing* match = dynamic_cast<RealThing*>(t);
                    if (match) {
                        currentThing = match;
                        createOrSelectThing();
                        Camera::panTo(currentThing->name);
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
            commandList->clearText();
            // Maybe "sprite" and "ray" should not be top level commands,
            // but instead "thing" should open up "TemplatePicker"
            // with a new thing
            if (input == "sprite") {
                changeState(EditorState::pathInput);
                return;
            }
            if (input == "free") {
                changeState(EditorState::freeMove);
                return;
            }
            if (input == "ray") {
                changeState(EditorState::rayEdit);
                return;
            }
            if (currentThing != dotThing) {
                // None of these should live here
                if (input == "rename") {
                    changeState(EditorState::renameThing);
                    return;
                }
                if (input == "move") {
                    changeState(EditorState::thingMove);
                    return;
                }
                if (input == "event") {
                    changeState(EditorState::eventEdit);
                    return;
                }
                if (input == "edit sprite") {
                    changeState(EditorState::spriteSelect);
                    return;
                }
                if (input == "copy") {
                    RealThing *newThing = new RealThing(*currentThing);
                    for (auto t : currentThing->subThings)
                        newThing->subThings.push_back(t->copyInPlace());
                    currentThing = newThing;
                    changeState(EditorState::thingMove);
                    return;
                }
                if (input == "delete") {
                    delete currentThing;
                    currentThing = dotThing;
                    changeState(EditorState::freeMove);
                    return;
                }
            }
            else {
                if (input == "play") {
                    changeState(EditorState::play);
                    return;
                }
                if (input == "template") {
                    changeState(EditorState::thingFromTemplate);
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
            if(addSprite()) {
                changeState(EditorState::spriteEdit);
            }
            else {
                lastPath = "";
                changeState(EditorState::commandInput);
            }
        }
    }

    if (state == EditorState::renameThing) {
        if (listenForTextInput(keysDown))
            return;
        if(keysDown.start) {
            string oldName = currentThing->name;
            if (input.length() > 0)
                eventMap::updateThingName(oldName, currentThing->rename(input));
            changeState(EditorState::commandInput);
        }
    }

    if(state == EditorState::spriteSelect) {
        if(keysDown.up || keysDown.debug_up) {
            selectedSprite = (selectedSprite + 1) % currentThing->sprites.size();
            currentThing->highlightSprite(currentThing->sprites[selectedSprite]);
            return;
        }
        if(keysDown.down || keysDown.debug_down) {
            selectedSprite = selectedSprite > 0 ? (selectedSprite - 1) : currentThing->sprites.size() - 1;
            currentThing->highlightSprite(currentThing->sprites[selectedSprite]);
            return;
        }
        if (keysDown.ok) {
            spriteEditor = new SpriteEditor(currentThing->sprites[selectedSprite]);
            selectedSprite = -1;
            changeState(EditorState::spriteEdit);
            return;
        }
        if (keysDown.menu2 && currentThing->sprites.size() > 1) {
            currentThing->RemoveSprite(currentThing->sprites[selectedSprite]);
            selectedSprite = selectedSprite > 0 ? (selectedSprite - 1) : 0;
            currentThing->highlightSprite(currentThing->sprites[selectedSprite]);
            return;
        }
        if (keysDown.cancel) {
            selectedSprite = -1;
            changeState(EditorState::commandInput);
            return;
        }
    }

    if(state == EditorState::spriteEdit) {
        if(spriteEditor->routeInput(keysDown)) {
            delete spriteEditor;
            spriteEditor = nullptr;
            currentThing->calculateHeight();
            changeState(EditorState::pathInput);
        }
    }

    if(state == EditorState::rayEdit) {
        if(rayEditor->routeInput(keysDown)) {
            delete rayEditor;
            rayEditor = nullptr;
            changeState(EditorState::commandInput);
            updateLines();
        }
    }

    if(state == EditorState::eventEdit) {
        if(eventEditor->routeInput(keysDown)) {
            delete eventEditor;
            eventEditor = nullptr;
            changeState(EditorState::commandInput);
        }
    }

    if(state == EditorState::thingFromTemplate) {
        if(templatePicker->chooseTemplate(keysDown)) {
            delete templatePicker;
            templatePicker = nullptr;
            changeState(EditorState::commandInput);
        }
    }
}


// Might want to refactor this to happen inside spriteEditor somehow
int MapBuilder::addSprite() {
    if(!SpriteEditor::checkPath(input))
        return 0;
    lastPath = input;
    createOrSelectThing();
    string path = "./assets/" + input;
    spriteEditor = new SpriteEditor(currentThing->AddRawSprite(path));
    spriteEditor->sprite->frontAndCenter();
    currentThing->highlightSprite(spriteEditor->sprite);

    return 1;
}