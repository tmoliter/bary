#include "SpriteEditor.h"

SpriteEditor::SpriteEditor(Sprite *s) : 
    sprite(s), 
    editSpeed(1), 
    editState(SpriteEditState::move), 
    cameraPrevState(SpriteEditState::move),
    foundSprite(false) {
    
    oldFocus = Thing::things[Camera::getFocusName()];
    focus = new Thing(Point(sprite->x, sprite->y), "sprite focus");
    Camera::panTo(focus->name);

    text = new Text(Point(sprite->x, sprite->y), "");
    UIRenderer::addText(text);
};

SpriteEditor::~SpriteEditor() {
    UIRenderer::removeText(text);
    Camera::panTo(oldFocus->name);
    delete focus;
};

int SpriteEditor::nextMode() {
    switch (editState) {
    case SpriteEditState::move:
        editState = SpriteEditState::shrink;
        return 0;
    case SpriteEditState::shrink:
        editState = SpriteEditState::grow;
        return 0;
    case SpriteEditState::grow:
        editState = SpriteEditState::layer;
        return 0;
    case SpriteEditState::layer:
        editState = SpriteEditState::renderOffset;
        return 0;
    case SpriteEditState::renderOffset:
        return 1;
    default:
        return 0;
    }
}

int SpriteEditor::lastMode() {
    switch (editState) {
    case SpriteEditState::move:
        return 0;
    case SpriteEditState::shrink:
        editState = SpriteEditState::move;
        return 0;
    case SpriteEditState::grow:
        editState = SpriteEditState::shrink;
        return 0;
    case SpriteEditState::layer:
        editState = SpriteEditState::grow;
        return 0;
    case SpriteEditState::renderOffset:
        editState = SpriteEditState::layer;
        return 0;
    default:
        return 0;
    }
}

void SpriteEditor::handleCameraControls(KeyPresses keysDown) {
    if (editState == SpriteEditState::cameraMove) {
        if (keysDown.menu2)
            editState = cameraPrevState;
        focus->manuallyControl(keysDown);
        return;
    }
    if (keysDown.menu2) {
        cameraPrevState = editState;
        editState = SpriteEditState::cameraMove;
    }
}

void SpriteEditor::changeEditSpeed(KeyPresses keysDown) {
    if(keysDown.debug_plus)
        editSpeed++;
    if(keysDown.debug_minus && editSpeed > 1)
        editSpeed--;
}

void SpriteEditor::frontAndCenter(KeyPresses keysDown) {
    if(keysDown.menu1){
        sprite->frontAndCenter();
    }
}

void SpriteEditor::displayText() {
    Point spritePos = sprite->getScreenPos(Camera::getPos());
    text->setPos(Point(spritePos.x, spritePos.y - (LETTER_HEIGHT * 2)));

    string displayText;
    switch (editState) {
    case SpriteEditState::move:
        displayText = "move";
        break;
    case SpriteEditState::shrink:
        displayText = "shrink";
        break;
    case SpriteEditState::grow:
        displayText = "grow";
        break;
    case SpriteEditState::layer:
        displayText = "layer: " + to_string(sprite->d.layer);
        break;
    case SpriteEditState::renderOffset:
        displayText = "render offset: " + to_string(sprite->d.renderOffset);
        break;
    case SpriteEditState::cameraMove:
        displayText = "camera move";
        break;
    }
    text->setText(displayText);
}

int SpriteEditor::routeInput(KeyPresses keysDown) {
    if(keysDown.ok)
        return nextMode();
    if(keysDown.cancel)
        return lastMode();
    handleCameraControls(keysDown);
    changeEditSpeed(keysDown);
    frontAndCenter(keysDown);
    displayText();
    switch (editState) {
    case SpriteEditState::move:
        move(keysDown);
        break;
    case SpriteEditState::shrink:
        shrink(keysDown);
        break;
    case SpriteEditState::grow:
        grow(keysDown);
        break;
    case SpriteEditState::layer:
        editLayer(keysDown);
        break;
    case SpriteEditState::renderOffset:
        editRenderOffset(keysDown);
        break;
    default:
        break;
    }
    return 0;
}

void SpriteEditor::move (KeyPresses keysDown) {
    if((keysDown.up || keysDown.debug_up)) {
        sprite->d.yOffset -= editSpeed;
    }
    if((keysDown.down || keysDown.debug_down)) {
        sprite->d.yOffset += editSpeed;
    }
    if((keysDown.left || keysDown.debug_left)) {
        sprite->d.xOffset -= editSpeed;
    }
    if((keysDown.right || keysDown.debug_right)) {
        sprite->d.xOffset += editSpeed;
    }
}

void SpriteEditor::shrink (KeyPresses keysDown) {
    if((keysDown.up || keysDown.debug_up) && sprite->d.height > editSpeed) {
        sprite->d.height -= editSpeed;
    }
    if((keysDown.down || keysDown.debug_down) && sprite->d.height > editSpeed) {
        sprite->d.height -= editSpeed;
        sprite->d.sourceY += editSpeed;
        sprite->d.yOffset += editSpeed;
    }
    if((keysDown.left || keysDown.debug_left) && sprite->d.width > editSpeed) {
        sprite->d.width -= editSpeed;
    }
    if((keysDown.right || keysDown.debug_right) && sprite->d.width > editSpeed) {
        sprite->d.width -= editSpeed;
        sprite->d.sourceX += editSpeed;
        sprite->d.xOffset += editSpeed;
    }
}

void SpriteEditor::grow (KeyPresses keysDown) {
    if((keysDown.up || keysDown.debug_up) && sprite->d.sourceY - editSpeed > 0) {
        sprite->d.height += editSpeed;
        sprite->d.sourceY -= editSpeed;
        sprite->d.yOffset -= editSpeed;
    }
    if((keysDown.down || keysDown.debug_down) && sprite->d.height + sprite->d.sourceY + editSpeed < sprite->sheetHeight) {
        sprite->d.height += editSpeed;
    }
    if((keysDown.left || keysDown.debug_left) && sprite->d.sourceX - editSpeed > 0) {
        sprite->d.width += editSpeed;
        sprite->d.sourceX -= editSpeed;
        sprite->d.xOffset -= editSpeed;
    }
    if((keysDown.right || keysDown.debug_right) && sprite->d.width + sprite->d.sourceX + editSpeed < sprite->sheetWidth) {
        sprite->d.width += editSpeed;
    }
}

void SpriteEditor::editLayer (KeyPresses keysDown) {
    if(keysDown.up || keysDown.debug_up)
        sprite->d.layer += editSpeed;
    if(keysDown.down || keysDown.debug_down)
        sprite->d.layer -= editSpeed;
}

void SpriteEditor::editRenderOffset (KeyPresses keysDown) {
    if(keysDown.up || keysDown.debug_up)
        sprite->d.renderOffset += editSpeed;
    if(keysDown.down || keysDown.debug_down)
        sprite->d.renderOffset -= editSpeed;
}

int SpriteEditor::checkPath(string input) {
    string possiblePath = string(BASE_PATH) + "assets/" + input;
    if(filesystem::path(possiblePath).extension() != ".png")
        return 0;
    const char* cPossiblePath = possiblePath.c_str();
    ifstream f(cPossiblePath);
    if(!f.good())
        return 0;
    return 1;
}