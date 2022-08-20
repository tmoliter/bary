#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <thread>
#include "Walk.h"

using namespace std;
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

void applySpeed(SDL_Rect &rect, int x, int y, int speed, int cycle) {
    if (speed == 0)
        return;
    if (speed < 3 && cycle % (3 - speed) != 0) {
        return;
    }
    int appliedSpeed = speed < 4 ? 1 : speed - 1;
    rect.x = rect.x + (x * appliedSpeed);
    rect.y = rect.y + (y * appliedSpeed);
};

void animate(SDL_Rect &rect, int x, int y, int speed, int cycle) {
    int totalFrames = 7;
    int delayPerFrame = 12 / speed;
    if (x != 0 || y != 0) {
        if(y > 0)
            rect.y = rect.h * 0;
        else if(y < 0)
            rect.y = rect.h * 1;
        else if(x < 0)
            rect.y = rect.h * 2;
        else if(x > 0)
            rect.y = rect.h * 3;

        int frame = ((cycle / delayPerFrame) % totalFrames) + 1;
        rect.x = frame * rect.w;
    }
    else if (rect.x != 0)
        rect.x = 0;
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = NULL;
    int a,b;
    Walk w = Walk(a, b);

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
        applySpeed(destRect,x,y,speed,cycle);
        animate(sourceRect,x,y,speed,cycle);

        SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, testTexture, &sourceRect, &destRect);
        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));

        cycle = cycle < INT_MAX ? cycle + 1 : 0;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(testTexture);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}