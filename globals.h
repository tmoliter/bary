#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "luaUtils.h"

inline int frameCount = 0;

inline char* BASE_PATH = SDL_GetBasePath();

inline struct Settings {
    void init()
    {
        lua_State* L = luaL_newstate();
        if (luaUtils::CheckLua(L, luaL_dofile(L, "config/settings.lua"))) {
            std::cout << "Loading settings..." << std::endl;
            lua_getglobal(L, "globals");
            luaUtils::GetLuaIntFromTable(L, "SCALE", SCALE);
            luaUtils::GetLuaIntFromTable(L, "SCREEN_WIDTH", SCREEN_WIDTH);
            luaUtils::GetLuaIntFromTable(L, "SCREEN_HEIGHT", SCREEN_HEIGHT);
            luaUtils::GetLuaBoolFromTable(L, "FULLSCREEN_MODE", FULLSCREEN_MODE);
            luaUtils::GetLuaIntFromTable(L, "LETTER_WIDTH", LETTER_WIDTH);
            luaUtils::GetLuaIntFromTable(L, "LETTER_HEIGHT", LETTER_HEIGHT);
            luaUtils::GetLuaIntFromTable(L, "LETTERS_PER_FONT_ROW", LETTERS_PER_FONT_ROW);
            SDL_Rect* RESOLUTION = new SDL_Rect {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
            lua_close(L);
        } else {
            std::cout << "couldn't find config/globals.lua" << std::endl;
        }
    }
    int SCALE;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int LETTER_WIDTH;
    int LETTER_HEIGHT;
    int LETTERS_PER_FONT_ROW;
    bool FULLSCREEN_MODE;
    SDL_Rect* RESOLUTION;
} settings;

inline SDL_Window* window;
inline SDL_Renderer* renderer;

inline enum class InputType {
    TextInput,
    Controller
} inputType;

enum class GameState {
    FieldFree,
    FieldUI,
    TextInput,
};
inline GameState gameState;

// These need their own namespace and ways of using other fonts/bubbles
inline static SDL_Texture *font = nullptr;
static inline SDL_Texture *defaultSpeechBubble = nullptr;

#endif
