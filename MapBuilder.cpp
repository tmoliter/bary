#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : editSpeed(1), attributeIndex(0), phase(Phase::freeMove) {
    currentThing = dotThing = new RealThing(Point(0,0));
    spriteText = nullptr;
    SpriteData sD;
    sD.height = 0;
    sD.width = 0;
    sD.path = "assets/debug/onePixel.png";
    currentSprite = new Sprite(
        currentThing->position.x, 
        currentThing->position.y, 
        currentThing->name, 
        sD
    );

    if (!font) {
        SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
        font = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }

    mapBuilder = this;
}

void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (phase == Phase::freeMove) {
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
            spriteText = new Text(Point(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 ), "");
            UIRenderer::addText(spriteText);
            input.clear();
            spriteText->clearText();
            gameState = GameState::TextInput;
            phase = Phase::pathInput;
            return;
        }
    }

    if (phase == Phase::pathInput) {
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
            if(addSprite()) {
                currentSprite->getInts(intAttrs, attrNames);
                phase = Phase::intInput;
                gameState = GameState::FieldFree;
            } else {
                gameState = GameState::FieldFree;
                phase = Phase::freeMove;
            }
            input.clear();
            spriteText->clearText();
            return;
        }
    }

    if(phase == Phase::intInput) {
        if(keysDown.debug_plus)
            editSpeed++;
        if(keysDown.debug_minus && editSpeed > 1)
            editSpeed--;
        if (keysDown.debug_up || keysDown.up)
            *intAttrs[attributeIndex] += editSpeed;
        if (keysDown.debug_down || keysDown.down)
            *intAttrs[attributeIndex] -= editSpeed;
        if(keysDown.ok && intAttrs.size() <= ++attributeIndex) {
            UIRenderer::removeText(spriteText);
            spriteText = nullptr;
            Camera::panTo(currentThing->name);
            attributeIndex = 0;
            currentSprite = nullptr;
            gameState = GameState::FieldFree;
            phase = Phase::freeMove;
            intAttrs.clear();
            return;
        }
        if(keysDown.cancel && attributeIndex > 0)
            attributeIndex--;
        string display = attrNames[attributeIndex] + " " + to_string(*intAttrs[attributeIndex]);
        Point spritePos = currentSprite->getScreenPos(Camera::getPos());
        spriteText->setPos(Point(spritePos.x, spritePos.y - (LETTER_HEIGHT * 2)));
        spriteText->setText(display);
    }
}

int MapBuilder::addSprite() {
    string possiblePath = string(BASE_PATH) + "assets/" + input;
    const char* cPossiblePath = possiblePath.c_str();
    ifstream f(cPossiblePath);
    if(!f.good())
        return 0;
    if (currentThing == dotThing) {
        currentThing = new RealThing(Point(currentThing->position.x, currentThing->position.y));
    }
    string path = "./assets/" + input;
    currentSprite = currentThing->AddRawSprite(path);
    return 1;
}