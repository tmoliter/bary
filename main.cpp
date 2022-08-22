#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "Background.h"
#include "FpsTimer.h"
#include "ThingList.h"
#include "globals.h"
#include <vector>
#include <algorithm>

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

    ThingList things;

    Background background = Background(NULL, NULL, renderer);

    FieldPlayer *player = new FieldPlayer(700, 700, &background.cameraX, &background.cameraY, renderer, "./assets/sheets/SDL_TestSS.png");
    things.AddThing(player);

    /* insane stress test */
    for (int i = 0; i < 20000; i++) {
        Thing *genrl = new Thing(rand() % 1000 + 100, rand() % 1000 + 100, &background.cameraX, &background.cameraY, renderer, "./assets/BurgGenrlL.png");
        things.AddThing(genrl);
    }
    // Thing *genrl = new Thing(500,500,&background.cameraX, &background.cameraY, renderer, "./assets/BurgGenrlL.png");
    // things.AddThing(genrl);

    background.setFocus(&player->x, &player->y);


    Input in;
    FpsTimer t;
    ProfileData p;
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();  
        t.timeElapsed(&p.a);

        if (keysDown.quit)
            break;
        things.MeatThings(keysDown);
        t.timeElapsed(&p.b);

        background.setPosition();
        background.render();
        t.timeElapsed(&p.c);

        things.RenderThings();
        t.timeElapsed(&p.d);

        SDL_RenderPresent(renderer);

        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount, p);
    }
    things.DestroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}