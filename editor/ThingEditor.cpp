#include "ThingEditor.h"

ThingEditor::ThingEditor() {
    thing = new RealThing(Point(0,0));
    changeState(ThingEditState::commandInput);
    commandLine = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
}

ThingEditor::~ThingEditor() {
    // Once we have a way to select spriteless things, we should remove this conditional
    // or expand it to also check for rays and events
    if (thing->sprites.size() < 1)
        delete thing;
    delete commandLine;
    delete spriteEditor;
    delete rayEditor;
    delete eventEditor;
    UIRenderer::removeText(helpText);
    RealThing::showAllLines();
}

void ThingEditor::changeState(ThingEditState newState) {
    switch (newState) {
        input = "";
        helpText->setText("");
        Sprite::highlightThing(thing->name);
        case ThingEditState::commandInput:
            commandLine = new CommandLine({
                "new sprite",
                "edit sprite", 
                "ray",
                "event",
                "rename",
                "copy",
                "delete"
                "free",
            }, false);
            break;
        case ThingEditState::move:
            helpText->setText(thing->name + ": Thing Move");
            Camera::panTo(thing->name);
            state = ThingEditState::move;
            break;
        case ThingEditState::pathInput:
            commandLine = new CommandLine({
                "Sprite Path"
            }, true);
            break;
        case ThingEditState::spriteEdit:
            helpText->setText(thing->name + ": Sprite Edit Mode");
            state = ThingEditState::spriteEdit;
            break;
        case ThingEditState::spriteSelect:
            helpText->setText(thing->name + + "Select/Delete Sprite ");
            selectedSprite = 0;
            thing->highlightSprite(thing->sprites[selectedSprite]);
            state = ThingEditState::spriteSelect;
            break;
        case ThingEditState::rayEdit:
            helpText->setText(thing->name + ": Ray Edit Mode");
            rayEditor = new RayEditor(thing);
            state = ThingEditState::rayEdit;
            break;
        case ThingEditState::eventEdit:
            helpText->setText(thing->name + ": Event Edit Mode");
            eventEditor = new EventEditor(thing);
            state = ThingEditState::eventEdit;
            break;
        case ThingEditState::rename:
            commandLine = new CommandLine({
                "Rename Thing"
            }, true);
            state = ThingEditState::rename;
            break;
        default:
            return;
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
        if (!commandLine->handleInput(keysDown))
            return 0;

        input = commandLine->handleInput(keysDown);
        if (input == "")
            return 0;
        delete commandLine;
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
            RealThing *newThing = new RealThing(*thing);
            for (auto t : thing->subThings)
                newThing->subThings.push_back(t->copyInPlace());
            thing = newThing;
            changeState(ThingEditState::move);
            return 0;
        }
        if (input == "delete") {
            delete thing;
            return 1;
        }
        if (input == "free") {
            return 1;
        }
        return 0;
    }

    if (state == ThingEditState::pathInput) {
        if (!commandLine->handleInput(keysDown))
            return 0;

        if (addSprite()) {
            delete commandLine;
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
    }

    if(state == ThingEditState::rayEdit) {
        if(rayEditor->routeInput(keysDown)) {
            delete rayEditor;
            rayEditor = nullptr;
            changeState(ThingEditState::commandInput);
            RealThing::hideAllLines();
            thing->showLines();
        }
    }

    if(state == ThingEditState::eventEdit) {
        if(eventEditor->routeInput(keysDown)) {
            delete eventEditor;
            eventEditor = nullptr;
            changeState(ThingEditState::commandInput);
        }
    }

    if (state == ThingEditState::rename) {
        if (!commandLine->handleInput(keysDown))
            return 0;

        input = commandLine->popInput();
        string oldName = thing->name;
        if (input.length() > 0)
            eventMap::updateThingName(oldName, thing->rename(input));
        changeState(ThingEditState::commandInput);
    }

}

int ThingEditor::addSprite() {
    input = commandLine->popInput();
    if(!SpriteEditor::checkPath(input))
        return 0;
    string path = "./assets/" + input;
    spriteEditor = new SpriteEditor(thing->AddRawSprite(path));
    spriteEditor->sprite->frontAndCenter();
    thing->highlightSprite(spriteEditor->sprite);
    return 1;
}