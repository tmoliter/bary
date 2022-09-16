#include "MapBuilder.h"

MapBuilder *MapBuilder::mapBuilder = nullptr;

MapBuilder::MapBuilder() : Thing(Point(0,0)) {
    SpriteData sD;
    sD.height = 0;
    sD.width = 0;
    sD.path = "assets/debug/onePixel.png";
    sprite = new Sprite(position.x, position.y ,name, sD);

    mapBuilder = this;
}

void MapBuilder::meat(KeyPresses keysDown) {
    DirectionMap dM;
    if (keysDown.up) 
        position.y-=3;
    if (keysDown.down)
        position.y+=3;
    if (keysDown.left)
        position.x-=3;
    if (keysDown.right)
        position.x+=3;
    if (keysDown.debug_up) 
        position.y--;
    if (keysDown.debug_down)
        position.y++;
    if (keysDown.debug_left)
        position.x--;
    if (keysDown.debug_right)
        position.x++;
}