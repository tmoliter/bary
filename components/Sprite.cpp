#include "Sprite.h"

using namespace std;

Sprite::Sprite (Point &pos, SpriteData sd) : 
    position(pos),
    d(sd),
    alpha(255),
    active(false) {
    id = currentID++;
    sprites[id] = this;
    texture = resourceDepository::getTexture(sd.textureName)->texture;
    SDL_QueryTexture(texture, NULL, NULL, &sheetWidth, &sheetHeight);
    d.width = sd.width > 0 ? sd.width : sheetWidth;
    d.height = sd.height > 0 ? sd.height : sheetHeight;
    active = true;
}

Sprite::Sprite(Sprite &sprite) :
    position(sprite.position),
    d(sprite.d),
    alpha(sprite.alpha),
    active(sprite.active),
    texture(sprite.texture),
    sheetWidth(sprite.sheetWidth),
    sheetHeight(sprite.sheetHeight) {
    id = currentID++;
    sprites[id] = this;
}

Sprite::Sprite(Sprite &sprite, Point &pos, string &tN) :
    position(pos),
    d(sprite.d),
    alpha(sprite.alpha),
    active(sprite.active),
    texture(sprite.texture),
    sheetWidth(sprite.sheetWidth),
    sheetHeight(sprite.sheetHeight) {
    id = currentID++;
    sprites[id] = this;
}

Sprite::~Sprite() {
    resourceDepository::releaseTexture(d.textureName);
    sprites.erase(id);
}

void Sprite::divideSheet(int columns, int rows) { 
    d.width = d.width / columns; 
    d.height = d.height / rows; 
}

void Sprite::centerOffset() {
    d.xOffset = -(d.width / 2);
    if(d.width % 2 == 0)
        d.xOffset--;
    d.yOffset = -(d.height / 2);
    if(d.height % 2 == 0)
        d.yOffset--;
}

void Sprite::frontAndCenter() {
    d.xOffset = -(d.width / 2);
    if(d.width % 2 == 0)
        d.xOffset--;
    d.yOffset = -d.height;
}

Point Sprite::getScreenPos(Point camPosition) {
    int renderX = ((position.x - camPosition.x) + d.xOffset) * settings.SCALE;
    int renderY = ((position.y - camPosition.y) + d.yOffset) * settings.SCALE;
    return Point(renderX, renderY);
}

void Sprite::render(SDL_Renderer *renderer, Point camPosition) {
    if (!active || texture == NULL)
        return;
    Point renderPos = getScreenPos(camPosition);
    SDL_Rect renderRect = { renderPos.x, renderPos.y, d.width * settings.SCALE, d.height * settings.SCALE };
    SDL_Rect sourceRect = { d.sourceX, d.sourceY, d.width, d.height };
    SDL_SetTextureAlphaMod(texture, alpha);
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
};

void Sprite::getInts(vector<int*> &ints, vector<string> &names) {
    vector<int*> spriteInts = vector<int*> { 
        &d.layer, 
        &d.renderOffset, 
        &d.width, 
        &d.height, 
        &d.sourceX, 
        &d.sourceY, 
        &d.xOffset, 
        &d.yOffset 
    };
    ints.insert(ints.end(), spriteInts.begin(), spriteInts.end());
    vector<string> spriteNames = vector<string> { 
        "layer",
        "renderOffset",
        "width",
        "height",
        "sourceX",
        "sourceY",
        "xOffset",
        "yOffset"
    };
    names.insert(names.end(), spriteNames.begin(), spriteNames.end());
}

// STATIC

int Sprite::currentID = 0;

bool _comparePosition (Sprite* a, Sprite* b) {
    if (a->d.layer == b->d.layer)
        return (
            a->position.y + a->d.height + a->d.yOffset - a->d.renderOffset < 
            b->position.y + b->d.height + b->d.yOffset - b->d.renderOffset
        );
    return a->d.layer < b->d.layer;
}
void Sprite::renderSprites(SDL_Renderer *renderer, Point camPosition) {
    vector<Sprite*> spriteList;
    for (auto const& [id, s] : Sprite::sprites)
        spriteList.push_back(s);
    sort(spriteList.begin(), spriteList.end(), _comparePosition);
    for (auto s : spriteList)
        s->render(renderer, camPosition);
}

void Sprite::highlightSprite(Sprite* sprite) {
    for (auto const& [id, s] : sprites)
        if (s != sprite)
            s->alpha = 100;
}

void Sprite::removeHighlight() {
    for (auto const& [id, s] : sprites)
        s->alpha = 255;
}
