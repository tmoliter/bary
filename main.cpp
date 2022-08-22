#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "Background.h"
#include "FpsTimer.h"
#include "ThingList.h"
#include "globals.h"
#include <vector>

using namespace std;

struct {
    int x = 300;
    int y = 400;
} genrlLoc;

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
    ThingList things;
    FieldPlayer *player = new FieldPlayer(playerX, playerY, &cameraX, &cameraY, renderer, "./assets/sheets/SDL_TestSS.png");
    player->divideSheet(9,4);
    things.AddThing(player);
    Thing *genrl = new Thing(genrlLoc.x,genrlLoc.y,&cameraX, &cameraY, renderer, "./assets/BurgGenrlL.png");
    things.AddThing(genrl);

    Input in;
    FpsTimer t;
    // ProfileData p;
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();  
        // t.timeSinceStart(&p.a);

        if (keysDown.quit)
            break;
        things.MeatThings(keysDown);
        // t.timeSinceStart(&p.b);

        background.setPosition();
        background.render();
        // t.timeSinceStart(&p.c);

        things.RenderThings();
        // t.timeSinceStart(&p.d);

        SDL_RenderPresent(renderer);

        t.endFrameAndWait(frameCount);
    }
    things.DestroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}