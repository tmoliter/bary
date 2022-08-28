#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Camera.h"
#include "FpsTimer.h"
#include "MapParser.h"
#include "globals.h"
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_Window* window = NULL;
    window = SDL_CreateWindow(
                "Timmy's Big Test",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
                );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    new Camera(renderer);

    parse_map("./maps/map2.txt");

    Input in;
    FpsTimer t;
    ProfileData p;

    while (true){
    t.startFrame();
        KeyPresses keysDown = in.getInput();  
        t.timeElapsed(&p.a);

        if (keysDown.quit)
            break;
        Thing::meatThings(keysDown);
        t.timeElapsed(&p.b);

        Camera::c->setPosition();
        Camera::c->render();

        t.timeElapsed(&p.c);

        Sprite::renderSprites();
        t.timeElapsed(&p.d);

        t.endFrameAndWait(frameCount);
        t.timeElapsed(&p.e);

        SDL_RenderPresent(renderer);
    }
    Thing::destroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete Camera::c;

    IMG_Quit();
    SDL_Quit();
    return 0;
}