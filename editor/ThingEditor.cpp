#include "ThingEditor.h"

ThingEditor::ThingEditor(Point p) {
    thing = Scene::currentScene->addThing(RealThingData(p));
    init();
}

ThingEditor::ThingEditor(RealThing *rt) {
    thing = rt;
    init();
}

ThingEditor::~ThingEditor() {
    // Once we have a way to select spriteless things, we should remove this conditional
    // or expand it to also check for rays and events
    if (thing != nullptr && thing->sprites.size() < 1)
        Scene::currentScene->destroyThing(thing);
    delete spriteEditor;
    delete rayEditor;
    delete eventEditor;
    UIRenderer::removeText(helpText);
    Scene::currentScene->showAllLines();
} 

void ThingEditor::init() {
    FocusTracker::ftracker->setFocus(thing);
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    helpText = new Text(Point(192, 16), "");
    UIRenderer::addText(helpText);
    changeState(ThingEditState::commandInput);
}

void ThingEditor::changeState(ThingEditState newState) {
    helpText->setText("");
    thing->highlightThing();
    CommandLine::breakdown();
    switch (newState) {
        case ThingEditState::commandInput:
            CommandLine::refresh({
                "move",
                "new sprite",
                "edit sprite", 
                "ray",
                "event",
                "rename",
                "copy",
                "delete",
                "free",
            }, CLIMode::typeCommand);
            helpText->setText(thing->name);
            break;
        case ThingEditState::move:
            helpText->setText(thing->name + ": Thing Move");
            FocusTracker::ftracker->setFocus(thing);
            break;
        case ThingEditState::pathInput:
            CommandLine::refresh({
                "Sprite Path"
            }, CLIMode::openText);
            break;
        case ThingEditState::spriteEdit:
            helpText->setText(thing->name + ": Sprite Edit Mode");
            break;
        case ThingEditState::spriteSelect:
            helpText->setText(thing->name + + "Select/Delete Sprite ");
            selectedSprite = 0;
            thing->highlightSprite(thing->sprites[selectedSprite]);
            break;
        case ThingEditState::rayEdit:
            helpText->setText(thing->name + ": Ray Edit Mode");
            rayEditor = new RayEditor(thing);
            break;
        case ThingEditState::eventEdit:
            helpText->setText(thing->name + ": Event Edit Mode");
            // eventEditor = new EventEditor(thing);
            break;
        case ThingEditState::rename:
            CommandLine::refresh({
                "Rename Thing"
            }, CLIMode::openText);
            break;
        default:
            break;
    }
    state = newState;
}

int ThingEditor::meat(KeyPresses keysDown) {

    if (state == ThingEditState::move) {
        thing->manuallyControl(keysDown);
        if(keysDown.start) {
            changeState(ThingEditState::commandInput);
            return 0;
        }
    }

    if (state == ThingEditState::commandInput) {
        if (!CommandLine::handleInput(keysDown))
            return 0;

        string input = CommandLine::popInput();
        if (input == "")
            return 0;
        if (input == "move") {
            changeState(ThingEditState::move);
            return 0;
        }
        if (input == "new sprite") {
            changeState(ThingEditState::pathInput);
            return 0;
        }
        if (input == "edit sprite") {
            if (thing->sprites.size() < 1)
                return 0;
            changeState(ThingEditState::spriteSelect);
            return 0;
        }
        if (input == "ray") {
            changeState(ThingEditState::rayEdit);
            return 0;
        }
        if (input == "event") {
            changeState(ThingEditState::eventEdit);
            return 0;
        }
        if (input == "rename") {
            changeState(ThingEditState::rename);
            return 0;
        }
        if (input == "copy") {
            RealThing *newThing = Scene::currentScene->copyThing(*thing);
            for (auto t : thing->subThings)
                newThing->subThings.push_back(t->copyInPlace());
            thing = newThing;
            changeState(ThingEditState::move);
            return 0;
        }
        if (input == "delete") {
            CommandLine::breakdown();
            Scene::currentScene->destroyThing(thing);
            thing = nullptr;
            return 1;
        }
        if (input == "free") {
            CommandLine::breakdown();
            return 1;
        }
        return 0;
    }

    if (state == ThingEditState::pathInput) {
        if (!CommandLine::handleInput(keysDown))
            return 0;

        if (addSprite()) {
            changeState(ThingEditState::spriteEdit);
            return 0;
        }
        return 0;
    }

    if (state == ThingEditState::spriteEdit) {
        if (spriteEditor->routeInput(keysDown)) {
            delete spriteEditor;
            spriteEditor = nullptr;
            thing->calculateHeight();
            changeState(ThingEditState::commandInput);
        }
        return 0;
    }

    if (state == ThingEditState::spriteSelect) {
        if(keysDown.up || keysDown.debug_up) {
            selectedSprite = (selectedSprite + 1) % thing->sprites.size();
            thing->highlightSprite(thing->sprites[selectedSprite]);
            return 0;
        }
        if(keysDown.down || keysDown.debug_down) {
            selectedSprite = selectedSprite > 0 ? (selectedSprite - 1) : thing->sprites.size() - 1;
            thing->highlightSprite(thing->sprites[selectedSprite]);
            return 0;
        }
        if (keysDown.ok) {
            spriteEditor = new SpriteEditor(thing->sprites[selectedSprite]);
            selectedSprite = -1;
            changeState(ThingEditState::spriteEdit);
            return 0;
        }
        if (keysDown.menu2 && thing->sprites.size() > 1) {
            thing->RemoveSprite(thing->sprites[selectedSprite]);
            selectedSprite = selectedSprite > 0 ? (selectedSprite - 1) : 0;
            thing->highlightSprite(thing->sprites[selectedSprite]);
            return 0;
        }
        if (keysDown.cancel) {
            selectedSprite = -1;
            changeState(ThingEditState::commandInput);
            return 0;
        }
        return 0;
    }

    if(state == ThingEditState::rayEdit) {
        if(rayEditor->routeInput(keysDown)) {
            delete rayEditor;
            rayEditor = nullptr;
            changeState(ThingEditState::commandInput);
            Scene::currentScene->hideAllLines();
            thing->showLines();
            return 0;
        }
    }

    if(state == ThingEditState::eventEdit) {
        // if(eventEditor->routeInput(keysDown)) { // tmp comment out while events are in turmoil
            // delete eventEditor;
            // eventEditor = nullptr;
            changeState(ThingEditState::commandInput);
            return 0;
        // }
    }

    if (state == ThingEditState::rename) {
        if (!CommandLine::handleInput(keysDown))
            return 0;

        string input = CommandLine::popInput();
        string oldName = thing->name;
        // Renaming will break lua event map
        changeState(ThingEditState::commandInput);
        return 0;
    }
    return 0;
}

int ThingEditor::addSprite() {
    string input = CommandLine::popInput();
    Sprite* sprite = thing->AddRawSprite(input);
    if (sprite->texture == NULL) {
        delete sprite;
        return 0;
    }
    spriteEditor = new SpriteEditor(sprite);
    spriteEditor->sprite->frontAndCenter();
    thing->highlightSprite(spriteEditor->sprite);
    return 1;
}