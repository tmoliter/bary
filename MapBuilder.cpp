#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : state(EditorState::freeMove) {
    mapBuilder = this;
    spriteText = nullptr;
    currentThing = dotThing = new RealThing(Point(0,0));
    SpriteData sD;
    sD.path = "assets/debug/onePixel.png";
    dotThing->AddSprite(new Sprite(
        currentThing->position.x, 
        currentThing->position.y, 
        currentThing->name, 
        sD
    ));
}

void MapBuilder::prepareForNextSprite() {
    spriteText = new Text(Point(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 ), "");
    UIRenderer::addText(spriteText);
    input.clear();
    spriteText->clearText();
    gameState = GameState::TextInput;
    state = EditorState::pathInput;
}


void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (state == EditorState::freeMove || state == EditorState::thingMove) {
        if (keysDown.up) 
            currentThing->position.y-=3;
        if (keysDown.down)
            currentThing->position.y+=3;
        if (keysDown.left)
            currentThing->position.x-=3;
        if (keysDown.right)
            currentThing->position.x+=3;
        if (keysDown.debug_up) 
            currentThing->position.y--;
        if (keysDown.debug_down)
            currentThing->position.y++;
        if (keysDown.debug_left)
            currentThing->position.x--;
        if (keysDown.debug_right)
            currentThing->position.x++;
        if (keysDown.cancel && currentThing != dotThing)
            currentThing = dotThing;
        if(keysDown.start) {
            prepareForNextSprite();
            return;
        }
    }

    if (state == EditorState::pathInput) {
        if (keysDown.textInput) {
            input.push_back(keysDown.textInput);
            spriteText->setText(input);
            return;
        }
        if (keysDown.del && input.length() > 0){
            input.pop_back();
            spriteText->setText(input);
            return;
        }
        if(keysDown.start) {
            gameState = GameState::FieldFree;
            if(addSprite()) {
                state = EditorState::spriteEdit;
            }
            else {
                currentThing->removeHighlight();
                currentThing = dotThing;
                state = EditorState::freeMove;
            }
            input.clear();
            spriteText->clearText();
            UIRenderer::removeText(spriteText);
            spriteText = nullptr;
            return;
        }
    }

    if(state == EditorState::spriteEdit) {
        if(spriteEditor->routeInput(keysDown)) {
            spriteEditor->sprite->alpha = 100;
            delete spriteEditor;
            prepareForNextSprite();
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