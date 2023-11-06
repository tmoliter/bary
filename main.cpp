#include "FpsTimer.h"
#include "MapParser.h"
#include "jukebox.h"

// include Lua headers
extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

using namespace std;

/* USEFUL LUA FUNCTIONS THAT SHOULD BE MOVED ELSEWHERE */
bool CheckLua(lua_State* L, int r) {
    if (r == LUA_OK) {
        return true;
    }
    string errmsg = lua_tostring(L, -1);
    cout << errmsg << endl;
    return false;
}

bool GetLuaStringFromTable(lua_State *L, string key, string &value, int tableIndex = -1) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isstring(L, -1)) {
            cout << key << " is not a string" << endl;
            return false;
        }
        value = lua_tostring(L, -1);
        lua_pop(L, 1);
        return true;
    }
    cout << "PROBLEM" << endl;
    return false;
}

bool GetLuaIntFromTable(lua_State *L, string key, int &value, int tableIndex = -1) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_isnumber(L, -1)) {
            cout << key << " is not a number" << endl;
            return false;
        }
        value = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return true;
    }
    cout << "PROBLEM" << endl;
    return false;
}

bool PushTableFromTable(lua_State *L, string key, int tableIndex = -1) {
    if (lua_istable(L, tableIndex)) {
        lua_pushstring(L, key.c_str());
        lua_gettable(L, tableIndex - 1);
        if (!lua_istable(L, -1)) {
            cout << key << " is not a table" << endl;
            return false;
        }
        return true;
    }
    cout << "PROBLEM" << endl;
    return false;
}

// Slightly modified to show negative indices from post:
// https://stackoverflow.com/questions/59091462/from-c-how-can-i-print-the-contents-of-the-lua-stack
static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i - top - 1, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

/* END USEFUL LUA FUNCTIONS THAT SHOULD BE MOVED ELSEWHERE */


RealThingData td; // FOR LUA TESTING

int main(int argc, char* args[]) {
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

    /* LUA TESTING */
    lua_State* L = luaL_newstate();
    if (CheckLua(L, luaL_dofile(L, "timmytesty.lua"))) {
        lua_getglobal(L, "thing");
        GetLuaIntFromTable(L, "x", td.x);
        GetLuaIntFromTable(L, "y", td.y);
        GetLuaStringFromTable(L, "name", td.name);
        vector<SpriteData> spriteDataVector;
        PushTableFromTable(L, "spriteDataVector");
        if(!lua_isnil(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                spriteDataVector.push_back(SpriteData());
                SpriteData &newSpriteData = spriteDataVector.back();
                GetLuaIntFromTable(L, "height", newSpriteData.height);
                GetLuaIntFromTable(L, "width", newSpriteData.width);
                GetLuaIntFromTable(L, "layer", newSpriteData.layer);
                GetLuaIntFromTable(L, "renderOffset", newSpriteData.renderOffset);
                GetLuaIntFromTable(L, "xOffset", newSpriteData.xOffset);
                GetLuaIntFromTable(L, "yOffset", newSpriteData.yOffset);
                GetLuaIntFromTable(L, "sourceX", newSpriteData.sourceX);
                GetLuaIntFromTable(L, "sourceY", newSpriteData.sourceY);
                GetLuaStringFromTable(L, "textureName", newSpriteData.textureName);
                lua_pop(L, 1);
            }
        }
        td.spriteDataVector = spriteDataVector;
        lua_pop(L, -1);
    } else {
        string errmsg = lua_tostring(L, -1);
        cout << errmsg << endl;
    }
    lua_close(L);
    RealThing* testyThing = new RealThing(td);
    /* END LUA TESTING */

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

    delete testyThing;
    delete Camera::c;

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    return 0;
}