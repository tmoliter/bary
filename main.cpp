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
#include "gui/UIRenderer.h"
#include "Event.h"
#include "EventNode.h"
#include "Timer.h"

using namespace std;

int test_event_node_callback () {
        Thing* player = Thing::things["Zinnia"];
        Timer::startOrIgnore("test");
        if(Timer::timeSince("test") < 60) {
            if (frameCount > 200)
                player->position.x++;
            player->position.y++;
            return 0;
        }
        Timer::destroy("test");
        return 1;
    };

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
    Phrase *ph = new Phrase(Point(100,100), 220, 80, 1, ScrollType::allButLast, "I think t.");
    Phrase *ph2 = new Phrase(Point(200,150), 300, 80, 2, ScrollType::allButLast, "Much less text.");
    EventNode *node, *node2;
    node = new EventNode(&node2, ph, &test_event_node_callback, &test_event_node_callback);
    node2 = new EventNode(NULL, ph2, &test_event_node_callback);
    Event* event = new Event();
    event->addNode(node);
    event->addNode(node2);
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
                if(frameCount == 10)
                    event->begin();
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