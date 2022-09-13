#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <vector>
#include "Camera.h"
#include "FpsTimer.h"
#include "MapParser.h"
#include "globals.h"
#include "gui/Phrase.h"
#include "gui/UIRenderer.h"
#include "Event.h"
#include "EventNode.h"
#include "Timer.h"
#include "./things/FieldPlayer.h"
#include "./things/Building.h"
#include "./components/SimpleMessageI.h"

using namespace std;

// int test_event_node_callback () {
//         Timer::startOrIgnore("test");
//         DirectionMap dM;
//         if(Timer::timeSince("test") < 60) {
//             if (frameCount > 350)
//                 dM.right = true;
//             dM.up = true;
//             FieldPlayer::player->walk->move(dM);
//             return 0;
//         }
//         Timer::destroy("test");
//         return 1;
//     };

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

    gameState = GameState::FieldFree;

    /* UI TESTING */
    // Phrase *ph = new Phrase(Point(100,100), 150, 40, 1, ScrollType::continuous, "What's that??`Is something up and to my right? I'd better go check this shit out.");
    // Phrase *ph2 = new Phrase(Point(200,150), 300, 24, 2, ScrollType::allButLast, "Damn, nothing.");
    // EventNode *node, *node2;
    // node = new EventNode(&node2, ph, &test_event_node_callback, &test_event_node_callback);
    // node2 = new EventNode(NULL, ph2, &test_event_node_callback);
    // Event* event = new Event();
    // event->addNode(node);
    // event->addNode(node2);

    Building* ss = Building::find_building("Sailor Shack");

    CollidableData cd;
    Ray *ray = new Ray(0,426,117,426);
    cd.rays.push_back(ray);
    cd.layer = 0;
    SimpleMessageI *smi = new SimpleMessageI(ss,"AHHH!!`I'd better run the fuck away from here dude!", cd);


    /* END UI TESTING */
    
    while (true){
    t.startFrame();
        KeyPresses keysDown = in.getInput();  
        if (keysDown.quit)
            break;
        switch(gameState) {
            case (GameState::FieldUI):
                Event::meat(keysDown);
                break;
            case (GameState::FieldFree):
            default:
                // if(frameCount == 300)
                //     event->begin();
                Thing::meatThings(keysDown);
                break;
        }

        Camera::c->render();

        UIRenderer::renderPhrases();

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