#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Camera.h"
#include "FpsTimer.h"
#include "MapParser.h"
#include "globals.h"
#include <vector>
#include <algorithm>
#include "gui/Phrase.h"

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

    string fullMapPath(string(BASE_PATH) + "maps/map2.txt");
    parse_map(fullMapPath.c_str());

    Input in;
    FpsTimer t;
    ProfileData p;

    Point pt = Point(0,0);

    Phrase ph1 = Phrase(pt, Point(100,100), 210, 83, 2, ScrollType::continuous, "I think that if I type out this whole sentence even with a stupendouslygigantanormous word it will display nicely even if it gets cut off eventually or something like that, but actually we need to make this a bit longer to test some other things.");
    // Phrase ph2 = Phrase(pt, Point(450,450), 300, 12, 1, "Here is some really tiny text.");
    // Phrase ph3 = Phrase(pt, Point(700,500), 40, 200, 4, "A message that is super big");

    while (true){
    t.startFrame();
        KeyPresses keysDown = in.getInput();  
        if (keysDown.quit)
            break;
        Thing::meatThings(keysDown);
        Camera::c->render();

        if (keysDown.debug_1)
            ph1.advance();
        ph1.progDisplay(6);
        // ph2.progDisplay(20);
        // ph3.progDisplay(35);
        t.endFrameAndWait(frameCount);
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