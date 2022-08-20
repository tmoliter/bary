#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"

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
    Input in;
    FieldPlayer t = FieldPlayer(1,2, renderer);
    t.init();

    int x = 0;
    int y = 0;
    int speed = 1;

    int cycle = 0;

    bool quit = false;
    while (!quit){
        Uint64 start = SDL_GetPerformanceCounter();
        KeyPresses keysDown = in.getInput();
        quit = keysDown.quit;
        SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
        SDL_RenderClear(renderer);


        t.incTick();
        t.meat(keysDown);
        t.render();

        SDL_RenderPresent(renderer);

        cycle = cycle < INT_MAX ? cycle + 1 : 0;
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}