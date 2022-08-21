#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "Background.h"
#include "FpsTimer.h"
#include "constants.h"

using namespace std;

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

    int playerX = 700, playerY = 700, cameraX, cameraY;
    Background background = Background(&cameraX, &cameraY, playerX, playerY, renderer); 
    FieldPlayer player = FieldPlayer(playerX, playerY, &cameraX, &cameraY, renderer);
    player.divideSheet(9,4);

    int frameCount = 0;
    Input in;
    FpsTimer t;
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();
        if (keysDown.quit)
            break;

        player.incTick();
        player.meat(keysDown);

        background.setPosition();
        background.render();
        player.render();

        SDL_RenderPresent(renderer);

        t.endFrameAndWait(frameCount);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}