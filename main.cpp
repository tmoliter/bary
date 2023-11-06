#include "FpsTimer.h"
#include "MapParser.h"
#include "jukebox.h"


// https://stackoverflow.com/questions/21616320/getting-linker-error-when-trying-to-import-lua-headers-into-c-project
// https://tuttlem.github.io/2014/01/08/getting-started-with-lua-using-c.html
// https://daley-paley.medium.com/super-simple-example-of-adding-lua-to-c-710730e9528a
// include Lua headers
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}


// *** define C++ function ***
static int MyCppFunction(lua_State* L) // Lua callable functions must be this format
{
    const int num = (int)lua_tonumber(L,1); // get first param from stack
    const char* str = lua_tostring(L,2); // get second param from stack
    std::cout << "Hello from C++!" << std::endl;
    std::cout << "num = " << num << ", str = " << str << std::endl;
    return 0; // how many params we're passing to Lua
}

using namespace std;

int main(int argc, char* args[]) {

    string cmd = "a = 7 + 11";
    lua_State* L = luaL_newstate();

    int r = luaL_dostring(L, cmd.c_str());
    if (r == LUA_OK) {
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1)) {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            cout << a_in_cpp << endl;
            cout << "SUCCESS" << endl;
        } else {
            cout << "NaN" << endl;
        }
    } else {
        string errmsg = lua_tostring(L, -1);
        cout << errmsg << endl;
    }

    // lua_State* L = luaL_newstate(); // create a new lua instance
    // luaL_openlibs(L); // give lua access to basic libraries
    // lua_register(L, "CallMyCppFunction", MyCppFunction); // register our C++ function with Lua
    // luaL_dofile(L, "main.lua"); // loads the Lua script
    
    // // *** call Lua function from C++ ***
    // lua_getglobal(L, "MyLuaFunction"); // find the Lua function
    // lua_pushnumber(L, 73); // push number as first param
    // lua_pushstring(L, "From C++ to Lua"); // push string as second param
    // lua_pcall(L, 2, 0, 0); // call the function passing 2 params

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if( Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(-1);
    }

    if( Mix_AllocateChannels(4) < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(-1);
    }

    // jukebox::playSong("Boss Battle", true);

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

    if (fullMapPath == string(BASE_PATH) + "maps/map2.txt")
        eventMap::load_events();

    Input in;
    FpsTimer t;
    ProfileData p;

    RealThing::showAllLines();

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
                men = new MenuDisplay({
                    Option("Mung Bean Juice", "Good for you. Not as good for the people sitting around you. Drink some when you need a cleanse."),
                    Option("Dollars", "Cash Rules Everything Around Me, CREAM it's a dollar"),
                    Option("Other Stuff", "Hey this is some other stuff, dude. Check it out sometime."),
                    Option("Ping Pong Paddle"),
                    Option("Gong"),
                    Option("Who Knows", "Why don't you take a peek down here and see? There's probably some really great stuff in here. Yeah, I bet if you just reach down and check it out, everything will be fine."),
                    Option("Some Garbage"),
                    Option("Key to My Butt"),
                    Option("Something Else"),
                    Option("Poop"),
                    Option("Dog Toy"),
                    Option("Bundle of Sticks"),
                    Option("Uncle John's Wig", "Made from horsehair. Full of Uncle John's hair.")
                },
                    Point(64, 100), 340, 60, 2
                );
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
            men->processInput(keysDown);
        }
        /* END MENU TESTING */

        
        /* NORMAL LOOP (DISABLE FOR MENU TESTING) */
        if (men == nullptr) {
            if(MapBuilder::mapBuilder)
                MapBuilder::mapBuilder->meat(keysDown);
            t.timeElapsed(&p.c);
            switch(gameState) {
                case (GameState::FieldUI):
                    Event::meat(keysDown);
                    break;
                case (GameState::FieldFree):
                default:
                    RealThing::meatThings(keysDown);
                    t.timeElapsed(&p.d);
                    break;
            }
        }
        /* END NORMAL LOOP DISABLED FOR MENU TESTING */

        FocusTracker::ftracker->setCameraFocalPoint();
        Camera::c->render();
        Sprite::renderSprites(renderer, Camera::c->getSourceRectCoords());

        UIRenderer::render();
        t.timeElapsed(&p.e);

        t.endFrameAndWait(frameCount, p);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);
        RealThing::destroyThings();
    }
    jukebox::stop();
    RealThing::destroyThings();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete Camera::c;

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}