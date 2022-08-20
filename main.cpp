#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <thread>
#include "components/Walk.h"

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

    SDL_Surface* temp = IMG_Load("./assets/sheets/SDL_TestSS.png");
    SDL_Texture* testTexture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);
    
    SDL_Event input;
    SDL_Rect destRect = { SCREEN_WIDTH / 2, 0, 64, 64 };
    SDL_Rect sourceRect = { 0, 0, 32, 32 };
    Walk w = Walk(destRect, sourceRect);

    int x = 0;
    int y = 0;
    int speed = 1;

    int cycle = 0;

    bool quit = false;
    while (!quit){
        Uint64 start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&input)){
            if (input.type == SDL_QUIT){
                quit = true;
            }
            if (input.type == SDL_KEYDOWN){
                switch( input.key.keysym.sym ){
                    case SDLK_a:
                        if (input.key.repeat == 0)
                            x += -1;
                        break;
                    case SDLK_d:
                        if (input.key.repeat == 0)
                            x += 1;
                        break;
                    case SDLK_w:
                        if (input.key.repeat == 0)
                            y += -1;
                        break;
                    case SDLK_s:
                        if (input.key.repeat == 0)
                            y += 1;
                        break;
                    case SDLK_UP:
                        if (input.key.repeat == 0)
                            speed += 1;
                        break;
                    case SDLK_DOWN:
                        if (input.key.repeat == 0)
                            speed = speed > 0 ? speed - 1 : 0;
                        break;
                }
            }
            if (input.type == SDL_KEYUP ){
                switch( input.key.keysym.sym ){
                    case SDLK_a:
                        x += 1;
                        break;
                    case SDLK_d:
                        x += -1;
                        break;
                    case SDLK_w:
                        y += 1;
                        break;
                    case SDLK_s:
                        y += -1;
                        break;
                }
            }
        }
        w.applySpeed(x,y,speed,cycle);
        w.animate(x,y,speed,cycle);

        SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, testTexture, &sourceRect, &destRect);
        SDL_RenderPresent(renderer);

        cycle = cycle < INT_MAX ? cycle + 1 : 0;
        
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(testTexture);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}