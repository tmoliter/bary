#include "barysystem.h"
#include "FpsTimer.h"
#include "editor/MapBuilder.h"

using namespace std;

int main(int argc, char* args[]) {
    barysystem::startup();
    MapBuilder *m = new MapBuilder("burg");
    // jukebox::playSong("Boss Battle", true);

    gameState = GameState::FieldFree;

    Input in;
    FpsTimer t;
    ProfileData p;

    /* MENU TESTING*/
    MenuDisplay* men = nullptr;
    /* END MENU TESTING*/

    while (true){
        t.startFrame();
        t.timeElapsed(&p.a);
        // TODO Should make Timer a singleton somehow so that we can time deeper down
        // and figure out why we're always stalling in `getInput()`
        KeyPresses keysDown = in.getInput();
        t.timeElapsed(&p.b);
        if (keysDown.quit)
            break;

        /* MENU TESTING */
        if (keysDown.debug_1) {
            if (men == nullptr) {
                men = new MenuDisplay({Option("Mung Bean Juice", "Good for you.")}, Point(64, 100), 340, 60, 2);
                men->addBox("pinkbox", {0, 0, 340, 120});
                men->addHeader("pinkinventoryheader", {0, 0, 340, 40});
                men->addFlavorBox("pinkinventoryfooter", {0, 0, 340, 80});
                UIRenderer::addMenuDisplay(men);
            } else {
                UIRenderer::removeMenuDisplay(men);
                men = nullptr;
            }
        }
        if (men != nullptr) {
            men->processInput(keysDown); // This needs to happen somewhere else. Are all menus in an event?
        }
        /* END MENU TESTING */

        if (men == nullptr) {
            if(MapBuilder::mapBuilder)
                MapBuilder::mapBuilder->meat(keysDown);
            t.timeElapsed(&p.c);
            switch(gameState) {
                // case (GameState::FieldUI):
                //     Event::meat(keysDown);
                //     break;
                case (GameState::FieldFree):
                default:
                    Scene::currentScene->meat(keysDown);
                    t.timeElapsed(&p.d);
                    break;
            }
        }

        FocusTracker::ftracker->setCameraFocalPoint();
        SDL_RenderClear(renderer);
        Camera::c->renderBackground();
        Sprite::renderSprites(renderer, Camera::c->getSourceRectCoords());
        UIRenderer::render();
        Camera::c->renderAfterEffects();


        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        
        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount, p);
        SDL_RenderPresent(renderer);
        Scene::currentScene->destroyThings();
    }
    jukebox::stop();
    Scene::currentScene->destroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete Camera::c;

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}