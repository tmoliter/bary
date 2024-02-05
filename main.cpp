#include "barysystem.h"
#include "FpsTimer.h"
#include "editor/MapBuilder.h"

using namespace std;

int main(int argc, char* args[]) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (!CheckLua(L, luaL_dofile(L, "scripts/load.lua")))
        throw exception();

    // jukebox::playSong("Boss Battle", true);

    gameState = GameState::FieldFree;

    Input in;
    FpsTimer t;
    ProfileData p;

    vector<string> saveNames;
    barysystem::startup(saveNames);
    lua_getglobal(L, "loadBaseResources");
    if(!luaUtils::CheckLua(L, lua_pcall(L, 0, 1, 0)))
        throw exception();
    resourceDepository::loadTexturesFromTable(L);
    lua_settop(L, 0);

    MenuDisplay* loadMenu = nullptr;
    vector<Option> startOptions;
    if (settings.BUILD_MODE)
        startOptions.push_back(Option("Editor", "Open Map Editor", "editor"));
    for (auto saveName : saveNames)
        startOptions.push_back(Option(saveName, "Load Save File", saveName));
    loadMenu = new MenuDisplay(startOptions, Point(64, 100), Point(340, 60), 2);
    loadMenu->addBox("defaultSpeechBubble", {0, 0, 640, 480});
    loadMenu->addFlavorBox("defaultSpeechBubble", {0, 0, 640, 480});
    UIRenderer::addMenuDisplay(loadMenu);

    while (true){
        t.startFrame();
        t.timeElapsed(&p.a);
        // TODO Should make Timer a singleton somehow so that we can time deeper down
        // and figure out why we're always stalling in `getInput()`
        KeyPresses keysDown = in.getInput();
        t.timeElapsed(&p.b);
        if (keysDown.quit)
            break;

        if (loadMenu) {
            string selection = loadMenu->getCurrentSelection().selectionText;
            if (loadMenu->processInput(keysDown, selection)) {
                if (selection == "Editor") {
                    UIRenderer::removeMenuDisplay(loadMenu);
                    loadMenu = nullptr;
                    MapBuilder *m = new MapBuilder("burg", L);
                } else {
                    UIRenderer::removeMenuDisplay(loadMenu);
                    loadMenu = nullptr;
                    lua_getglobal(L, "loadGame");
                    lua_pushstring(L, selection.c_str());
                    if(!luaUtils::CheckLua(L, lua_pcall(L, 1, 1, 0)))
                        throw exception();
                    string sceneName;
                    string spawnName;
                    luaUtils::GetLuaStringFromTable(L, "scene", sceneName);
                    luaUtils::GetLuaStringFromTable(L, "name", spawnName);
                    luaUtils::GetLuaIntFromTable(L, "scale", settings.SCALE);
                    Scene* scene = new Scene(sceneName, L);
                    scene->Load(false);
                    scene->EnterLoaded(scene->things[spawnName]);
                    lua_settop(L, 0);
                    gameState = GameState::FieldFree;
                }
            }
            SDL_RenderClear(renderer);
            UIRenderer::render();
        } else if (Scene::currentScene) {
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
        } else {
            cout << "No load menu or current scene. What are we doing?" << endl;
            throw exception();
        }
        lua_settop(L, 0);
        t.timeElapsed(&p.e);
        t.endFrameAndWait(frameCount, p);

        SDL_RenderPresent(renderer);
        
        if (loadMenu == nullptr)
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