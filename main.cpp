#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "fpsTimer.h"

using namespace std;
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = NULL;
    window = SDL_CreateWindow(
                "Timmy's Big Test",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
                );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    FieldPlayer player = FieldPlayer(1,2, renderer);
    player.init();

    int frameCount = 0;
    Input in;
    fpsTimer t;
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();

        player.incTick();
        player.meat(keysDown);
        player.render();

        SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        t.endFrameAndWait(frameCount);
        if (keysDown.quit)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}