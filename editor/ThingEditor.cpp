#include "ThingEditor.h"

ThingEditor::ThingEditor(Point p) {
    thing = new RealThing(p);
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
        delete thing;
    delete spriteEditor;
    delete rayEditor;
    delete eventEditor;
    UIRenderer::removeText(helpText);
    RealThing::showAllLines();
}

void ThingEditor::init() {
    Camera::panTo(thing->name, true);
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    helpText = new Text(Point(192, 16), "");
    UIRenderer::addText(helpText);
    changeState(ThingEditState::commandInput);
}

void ThingEditor::changeState(ThingEditState newState) {
    helpText->setText("");
    Sprite::highlightThing(thing->name);
    CommandLine::breakdown();
    switch (newState) {
        case ThingEditState::commandInput:
            CommandLine::refresh({
                "new sprite",
                "edit sprite", 
                "ray",
                "event",
                "rename",
                "copy",
                "delete",
                "free",
            }, false);
            helpText->setText(thing->name);
            break;
        case ThingEditState::move:
            helpText->setText(thing->name + ": Thing Move");
            Camera::panTo(thing->name, true);
            break;
        case ThingEditState::pathInput:
            CommandLine::refresh({
                "Sprite Path"
            }, true);
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
            eventEditor = new EventEditor(thing);
            break;
        case ThingEditState::rename:
            CommandLine::refresh({
                "Rename Thing"
            }, true);
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
            RealThing *newThing = new RealThing(*thing);
            for (auto t : thing->subThings)
                newThing->subThings.push_back(t->copyInPlace());
            thing = newThing;
            changeState(ThingEditState::move);
            return 0;
        }
        if (input == "delete") {
            CommandLine::breakdown();
            delete thing;
            thing = nullptr;
            return 1;
        }
        if (input == "free") {
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
            RealThing::hideAllLines();
            thing->showLines();
            return 0;
        }
    }

    if(state == ThingEditState::eventEdit) {
        if(eventEditor->routeInput(keysDown)) {
            delete eventEditor;
            eventEditor = nullptr;
            changeState(ThingEditState::commandInput);
            return 0;
        }
    }

    if (state == ThingEditState::rename) {
        if (!CommandLine::handleInput(keysDown))
            return 0;

        string input = CommandLine::popInput();
        string oldName = thing->name;
        if (input.length() > 0)
            eventMap::updateThingName(oldName, thing->rename(input));
        changeState(ThingEditState::commandInput);
        return 0;
    }
    return 0;
}

int ThingEditor::addSprite() {
    string input = CommandLine::popInput();
    if(!SpriteEditor::checkPath(input))
        return 0;
    string path = "./assets/" + input;
    spriteEditor = new SpriteEditor(thing->AddRawSprite(path));
    spriteEditor->sprite->frontAndCenter();
    thing->highlightSprite(spriteEditor->sprite);
    return 1;
}