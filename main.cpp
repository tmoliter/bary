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
                "Bary",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
                );

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    string total(string(BASE_PATH) + "maps/map2.txt");
    parse_map(total.c_str());

    Input in;
    FpsTimer t;
    ProfileData p;

    while (true){
    t.startFrame();
        KeyPresses keysDown = in.getInput();  
        if (keysDown.quit)
            break;
        Thing::meatThings(keysDown);
        Camera::c->render();
        t.endFrameAndWait(frameCount);
        SDL_RenderPresent(renderer);
        Thing::destroyThings();
    }
    Thing::destroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete Camera::c;

    IMG_Quit();
    SDL_Quit();
    return 0;
}