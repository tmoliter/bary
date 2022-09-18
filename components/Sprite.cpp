#include "Sprite.h"

using namespace std;

Sprite::Sprite (int &x, int &y, string &tN, SpriteData sd) : 
x(x), 
y(y), 
thingName(tN), 
d(sd),
active(false) {
    id = currentID++;
    sprites[id] = this;
    if(!textures.count(sd.path)) {
        SDL_Surface* temp = IMG_Load(sd.path.c_str());
        textures[sd.path] = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    texture = textures[sd.path];
    SDL_QueryTexture(texture, NULL, NULL, &d.width, &d.height);
    if(sd.width > 0)
        d.width = sd.width;
    if(sd.height > 0)
        d.height = sd.height;
    active = true;
}

Sprite::~Sprite() {
    SDL_DestroyTexture(texture);
    sprites.erase(id);
}

void Sprite::divideSheet(int columns, int rows) { 
    d.width = d.width / columns; 
    d.height = d.height / rows; 
}

void Sprite::render(SDL_Renderer *renderer, Point camPosition) {
    if (!active)
        return;
    int renderX = ((x - camPosition.x) + d.xOffset) * SCALE;
    int renderY = ((y - camPosition.y) + d.yOffset) * SCALE;
    SDL_Rect renderRect = { renderX, renderY, d.width * SCALE, d.height * SCALE };
    SDL_Rect sourceRect = { d.sourceX, d.sourceY, d.width, d.height };
    SDL_RenderCopy(renderer, texture, &sourceRect, &renderRect);
};

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
    for (auto const& [id, sprite] : Sprite::sprites){
        spriteList.push_back(sprite);
    }
    sort(spriteList.begin(), spriteList.end(), _comparePosition);
    for (auto sprite : spriteList){
        sprite->render(renderer, camPosition);
    }
}
int Sprite::write_sprite_datum(ifstream &mapData, SpriteData &newSD){
    int index = 0;
    string value = "";
    char current;
    utils::parse_ints(vector<int*> { &newSD.layer, &newSD.renderOffset, &newSD.width, &newSD.height, &newSD.sourceX, &newSD.sourceY, &newSD.xOffset, &newSD.yOffset }, mapData);
    while(mapData.get(current)) {
            if (current == ',') {
            cout << value << endl << endl;
            newSD.path = value;
            return 1;
        }
        value.push_back(current);
    }
    return 1;
}