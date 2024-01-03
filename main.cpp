#include "barysystem.h"
#include "FpsTimer.h"
#include "editor/MapBuilder.h"

using namespace std;

int main(int argc, char* args[]) {
    vector<string> saveNames;
    barysystem::startup(saveNames);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (!CheckLua(L, luaL_dofile(L, "scripts/load.lua")))
        throw exception();

    // jukebox::playSong("Boss Battle", true);

    gameState = GameState::FieldFree;

    Input in;
    FpsTimer t;
    ProfileData p;

    /* MENU TESTING*/
    MenuDisplay* men = nullptr;
    /* END MENU TESTING*/

    string sceneName = "";
    vector<Option> startOptions = { Option("Editor") };
    for (auto saveName : saveNames)
        startOptions.push_back(Option(saveName));
    men = new MenuDisplay(startOptions, Point(64, 100), 340, 60, 2);
    men->addBox("pinkbox", {0, 0, 340, 120});
    UIRenderer::addMenuDisplay(men);

    while (true){
        t.startFrame();
        t.timeElapsed(&p.a);
        // TODO Should make Timer a singleton somehow so that we can time deeper down
        // and figure out why we're always stalling in `getInput()`
        KeyPresses keysDown = in.getInput();
        t.timeElapsed(&p.b);
        if (keysDown.quit)
            break;

        if (men == nullptr) {
            if(MapBuilder::mapBuilder)
                MapBuilder::mapBuilder->meat(keysDown);
            t.timeElapsed(&p.c);
            switch(gameState) {
                case (GameState::FieldFree):
                default:
                    Scene::currentScene->meat(keysDown);
                    t.timeElapsed(&p.d);

                    FocusTracker::ftracker->setCameraFocalPoint();
                    SDL_RenderClear(renderer);
                    Camera::c->renderBackground();
                    Sprite::renderSprites(renderer, Camera::c->getSourceRectCoords());
                    UIRenderer::render();
                    Camera::c->renderAfterEffects();

                    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                    break;
            }
        }

        if (men != nullptr) {
            string selection = men->getCurrentSelection().selectionText;
            if (men->processInput(keysDown, selection)) {
                cout << selection << endl;
                if (selection == "Editor") {
                    UIRenderer::removeMenuDisplay(men);
                    men = nullptr;
                    MapBuilder *m = new MapBuilder("burg", L);
                } else {
                    lua_getglobal(L, "loadGame");
                    lua_pushstring(L, selection.c_str());
                    if(!luaUtils::CheckLua(L, lua_pcall(L, 1, 0, 0)))
                        throw exception();
                    
                }
            }
            SDL_RenderClear(renderer);
            UIRenderer::render();
        }

        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount, p);

        SDL_RenderPresent(renderer);
        
        if (men == nullptr)
            Scene::currentScene->destroyThings();
    }
    jukebox::stop();
    if (Scene::currentScene)
        delete Scene::currentScene;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete Camera::c;

    Mix_CloseAudio();
    lua_close(L);
    IMG_Quit();
    SDL_Quit();
    return 0;
}