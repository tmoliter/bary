#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "things/FieldPlayer.h"
#include "Camera.h"
#include "FpsTimer.h"
#include "ThingList.h"
#include "MapParser.h"
#include "globals.h"
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    new Camera();

    ThingList things;
    parse_map(things);

    Input in;
    FpsTimer t;
    ProfileData p;
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();  
        t.timeElapsed(&p.a);

        if (keysDown.quit)
            break;
        things.meatThings(keysDown);
        t.timeElapsed(&p.b);

        Camera::c->setPosition();
        Camera::c->render();

        t.timeElapsed(&p.c);

        things.renderThings();
        t.timeElapsed(&p.d);

        SDL_RenderPresent(Camera::c->renderer);

        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount);
    }
    SDL_DestroyRenderer(Camera::c->renderer);
    SDL_DestroyWindow(Camera::c->window);
    things.destroyThings();
    delete Camera::c;
    IMG_Quit();
    SDL_Quit();
    return 0;
}