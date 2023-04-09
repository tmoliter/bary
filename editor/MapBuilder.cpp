#include "MapBuilder.h"
#include <SDL2/SDL_mixer.h>

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : selectedSprite(-1) {
    mapBuilder = this;
    cross = nullptr;
    spriteEditor = nullptr;
    rayEditor = nullptr;
    eventEditor = nullptr;
    thingRouter = nullptr;

    CommandLine::init();

    // Maybe we can pass this down into Editors to share? Maybe that's dumb.
    helpText = new Text(Point(16, 16), "");
    UIRenderer::addText(helpText);
    
    currentThing = dotThing = new RealThing(Point(0,0), "EditorDot");
    dotThing->AddRawSprite("assets/debug/onePixel.png");
    changeState(EditorState::freeMove);
}

void MapBuilder::changeState(EditorState newState) {
    string prefix = currentThing != dotThing ? currentThing->name + ": " : "";
    switch (newState) {
        case EditorState::freeMove:
            helpText->setText("Free Move");
            focusDot();
            Sprite::removeHighlight();
            state = EditorState::freeMove;
            break;
        case EditorState::play:
            helpText->setText("Play");
            currentThing = new FieldPlayer(dotThing->position, "test player", "./assets/sheets/SDL_TestSS.png");
            Camera::panTo(currentThing->name, true);
            state = EditorState::play;
            break;
        case EditorState::commandInput:
            helpText->setText("");
            CommandLine::refresh({"play", "new thing", "free"}, false);
            state = EditorState::commandInput;
            break;
        case EditorState::thingEdit:
            helpText->clearText();
            state = EditorState::thingEdit;
            break;
    }
    updateLines();
}

void MapBuilder::focusDot() {
    currentThing = dotThing;
    Camera::panTo(currentThing->name, true);
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

void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (state == EditorState::freeMove) {
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
                    RealThing* match = dynamic_cast<RealThing*>(t);
                    // TODO should actually make a vector out of ALL matches and pass them in so we can choose which one we want to deal with
                    if (match) {
                        thingRouter = new ThingRouter(match);
                        changeState(EditorState::thingEdit);
                        return;
                    }
                }
            }
        }
    }


    if (state == EditorState::commandInput) {
        if (!CommandLine::handleInput(keysDown))
            return;
        string input = CommandLine::popInput();
        if (input == "")
            return;
        CommandLine::breakdown();
        if (input == "play") {
            changeState(EditorState::play);
            return;
        }
        if (input == "free") {
            changeState(EditorState::freeMove);
            return;
        }
        if (input == "new thing") {
            thingRouter = new ThingRouter(currentThing->position);
            changeState(EditorState::thingEdit);
            return;
        }
    }

    if(state == EditorState::thingEdit) {
        if(thingRouter->routeInput(keysDown)) {
            delete thingRouter;
            thingRouter = nullptr;
            changeState(EditorState::freeMove);
        }
    }
}
