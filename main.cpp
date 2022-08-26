#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "Camera.h"
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

    Camera *camera = new Camera(NULL, NULL, renderer);

    ThingData genrlData;
    genrlData.id = 1;
    genrlData.x = 700;
    genrlData.y = 700;
    genrlData.path = "./assets/BurgGenrlL.png";
    Thing *genrl = new Thing(genrlData);
    genrl->init(&camera->x, &camera->y, renderer);
    things.AddThing(genrl);

    FieldPlayerData playerData;
    playerData.id = 0;
    playerData.x = 700;
    playerData.y = 700;
    playerData.name = "Zinnia";
    playerData.path = "./assets/sheets/SDL_TestSS.png";
    FieldPlayer *player = new FieldPlayer(playerData);
    player->init(&camera->x, &camera->y, renderer);
    things.AddThing(player);

    camera->setFocus(&player->x, &player->y);

    /* insane stress test */
    // for (int i = 0; i < 20000; i++) {
    //     Thing *genrl = new Thing(rand() % 1000 + 100, rand() % 1000 + 100, &camera.x, &camera.y, renderer, "./assets/BurgGenrlL.png");
    //     things.AddThing(genrl);
    // }

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

        camera->setPosition();
        camera->render();
        t.timeElapsed(&p.c);

        things.RenderThings();
        t.timeElapsed(&p.d);

        SDL_RenderPresent(renderer);

        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    things.DestroyThings();
    delete camera;
    IMG_Quit();
    SDL_Quit();
    return 0;
}