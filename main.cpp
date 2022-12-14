#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <vector>
#include "globals.h"
#include "Camera.h"
#include "FpsTimer.h"
#include "MapParser.h"
#include "gui/UIRenderer.h"
#include "events/eventMap.h"

using namespace std;

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_Window* window = SDL_CreateWindow(
            "Bary",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            fullscreen_mode ? 0 : SCREEN_WIDTH, fullscreen_mode ? 0 :SCREEN_HEIGHT,
            fullscreen_mode ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN
            );

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (fullscreen_mode) {
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    string fullMapPath(string(BASE_PATH) + "maps/edit.txt");
    parse_map(fullMapPath.c_str());
    eventMap::buildEventMap();

    gameState = GameState::FieldFree;

    // if (fullMapPath == string(BASE_PATH) + "maps/map2.txt")
    //     eventMap::load_events();

    Input in;
    FpsTimer t;
    ProfileData p;

    RealThing::showAllLines();
    
    while (true){
        t.startFrame();
        KeyPresses keysDown = in.getInput();  
        t.timeElapsed(&p.a);
        if (keysDown.quit)
            break;
        t.timeElapsed(&p.b);
        
        if(MapBuilder::mapBuilder)
            MapBuilder::mapBuilder->meat(keysDown);
        t.timeElapsed(&p.c);
        switch(gameState) {
            case (GameState::FieldUI):
                Event::meat(keysDown);
                break;
            case (GameState::FieldFree):
            default:
                Thing::meatThings(keysDown);
                t.timeElapsed(&p.d);
                break;
        }

        Camera::c->render();

        UIRenderer::render();
        t.timeElapsed(&p.e);

        t.endFrameAndWait(frameCount, p);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
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