#include "SpriteEditor.h"

SpriteEditor::SpriteEditor(Sprite *s) : sprite(s), editSpeed(1), editState(EditState::move) {
    SpriteData sd;
    sd.path = "./assets/debug/9x9cross.png";
    sd.layer = 100;
    cross = new Sprite(sprite->x, sprite->y, sprite->thingName, sd);
    cross->frontAndCenter();
    text = new Text(Point(sprite->x, sprite->y), "");
    UIRenderer::addText(text);
};

SpriteEditor::~SpriteEditor() {
    UIRenderer::removeText(text);
    delete cross;
};

int SpriteEditor::nextMode() {
    switch (editState) {
    case EditState::move:
        editState = EditState::shrink;
        return 0;
    case EditState::shrink:
        editState = EditState::grow;
        return 0;
    case EditState::grow:
        editState = EditState::layer;
        return 0;
    case EditState::layer:
        editState = EditState::renderOffset;
        return 0;
    case EditState::renderOffset:
        return 1;
    }
}

int SpriteEditor::lastMode() {
    switch (editState) {
    case EditState::move:
        return 0;
    case EditState::shrink:
        editState = EditState::move;
        return 0;
    case EditState::grow:
        editState = EditState::shrink;
        return 0;
    case EditState::layer:
        editState = EditState::grow;
        return 0;
    case EditState::renderOffset:
        editState = EditState::layer;
        return 0;
    }
}

void SpriteEditor::changeEditSpeed(KeyPresses keysDown) {
    if(keysDown.debug_plus)
        editSpeed++;
    if(keysDown.debug_minus && editSpeed > 1)
        editSpeed--;
}

void SpriteEditor::frontAndCenter(KeyPresses keysDown) {
    if(keysDown.menu1)
        sprite->frontAndCenter();
}

void SpriteEditor::displayText() {
    Point spritePos = sprite->getScreenPos(Camera::getPos());
    text->setPos(Point(spritePos.x, spritePos.y - (LETTER_HEIGHT * 2)));

    string displayText;
    switch (editState) {
    case EditState::move:
        displayText = "move";
        break;
    case EditState::shrink:
        displayText = "shrink";
        break;
    case EditState::grow:
        displayText = "grow";
        break;
    case EditState::layer:
        displayText = "layer: " + to_string(sprite->d.layer);
        break;
    case EditState::renderOffset:
        displayText = "render offset: " + to_string(sprite->d.renderOffset);
        break;
    }
    text->setText(displayText);
}

int SpriteEditor::routeInput(KeyPresses keysDown) {
    if(keysDown.ok)
        return nextMode();
    if(keysDown.cancel)
        return lastMode();
    changeEditSpeed(keysDown);
    frontAndCenter(keysDown);
    displayText();
    switch (editState) {
    case EditState::move:
        move(keysDown);
        break;
    case EditState::shrink:
        shrink(keysDown);
        break;
    case EditState::grow:
        grow(keysDown);
        break;
    case EditState::layer:
        editLayer(keysDown);
        break;
    case EditState::renderOffset:
        editRenderOffset(keysDown);
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