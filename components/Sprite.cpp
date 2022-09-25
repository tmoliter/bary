#include "Sprite.h"

using namespace std;

Sprite::Sprite (int &x, int &y, string &tN, SpriteData sd) : 
x(x), 
y(y), 
thingName(tN), 
d(sd),
alpha(255),
active(false) {
    id = currentID++;
    sprites[id] = this;
    if(!textures.count(sd.path)) {
        SDL_Surface* temp = IMG_Load(d.path.c_str());
        textures[d.path].second = SDL_CreateTextureFromSurface(renderer, temp);
        textures[d.path].first = 1;
        SDL_FreeSurface(temp);
    } else {
        textures[d.path].first++;
    }
    texture = textures[d.path].second;
    SDL_QueryTexture(texture, NULL, NULL, &sheetWidth, &sheetHeight);
    d.width = sd.width > 0 ? sd.width : sheetWidth;
    d.height = sd.height > 0 ? sd.height : sheetHeight;
    active = true;
}

Sprite::~Sprite() {
    textures[d.path].first--;
    if (textures[d.path].first < 1) {
        SDL_DestroyTexture(texture);
        textures.erase(d.path);
    }
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
    int renderX = ((x - camPosition.x) + d.xOffset) * SCALE;
    int renderY = ((y - camPosition.y) + d.yOffset) * SCALE;
    return Point(renderX, renderY);
}

void Sprite::render(SDL_Renderer *renderer, Point camPosition) {
    if (!active)
        return;
    Point renderPos = getScreenPos(camPosition);
    SDL_Rect renderRect = { renderPos.x, renderPos.y, d.width * SCALE, d.height * SCALE };
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
            a->y + a->d.height + a->d.yOffset - a->d.renderOffset < 
            b->y + b->d.height + b->d.yOffset - b->d.renderOffset
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

int Sprite::parse_sprite_datum(ifstream &mapData, SpriteData &newSD){
    utils::parse_ints(vector<int*> { 
        &newSD.layer,
        &newSD.renderOffset,
        &newSD.width,
        &newSD.height,
        &newSD.sourceX,
        &newSD.sourceY,
        &newSD.xOffset,
        &newSD.yOffset
    }, mapData);
    utils::parse_strings(vector<string*> { &newSD.path }, mapData);
    return 1;
}

void Sprite::highlightSprite(Sprite* sprite) {
    for (auto const& [id, s] : sprites)
        if (s != sprite)
            s->alpha = 100;
}

void Sprite::highlightThing(string thingName) {
    for (auto const& [id, s] : sprites)
        if (s->thingName != thingName)
            s->alpha = 100;
}

void Sprite::removeHighlight() {
    for (auto const& [id, s] : sprites)
        s->alpha = 255;
}
