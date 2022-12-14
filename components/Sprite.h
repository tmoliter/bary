#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <../include/SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "globals.h"
#include "Ray.h"
#include "utils.h"

using namespace std;

struct SpriteData {
    int layer = 0;
    int renderOffset = 0;
    int xOffset = 0;
    int yOffset = 0;
    int sourceX = 0;
    int sourceY = 0;
    int width = 0;
    int height = 0;
    string path = "";
};

struct Sprite {
    Sprite(Point &pos, string &tN, SpriteData sd);
    Sprite(Sprite &sprite);
    Sprite(Sprite &sprite, Point &pos, string &tN);
    ~Sprite();
    bool active;
    int id;
    int alpha, sheetHeight, sheetWidth;
    Point &position;
    SpriteData d;
    string &thingName;
    SDL_Texture* texture;

    void divideSheet(int columns, int rows);
    void centerOffset();
    void frontAndCenter();
    Point getScreenPos(Point camPosition);
    virtual void render(SDL_Renderer *renderer, Point camPosition);

    void getInts(vector<int*> &ints, vector<string> &names);

    static int currentID;
    inline static map<int, Sprite*> sprites;
    inline static map<string, pair<int, SDL_Texture*>> textures;
    static void renderSprites(SDL_Renderer *renderer, Point camPosition);

    static void highlightSprite(Sprite* sprite);
    static void highlightThing(string thingName);
    static void removeHighlight();


    static int parse_sprite_datum(ifstream &mapData, SpriteData &newSD);
};

#endif
