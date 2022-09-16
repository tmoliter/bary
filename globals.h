#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL2/SDL.h>

inline int frameCount = 0;

inline char* BASE_PATH = SDL_GetBasePath();

inline constexpr int SCALE = 2;
inline constexpr int SCREEN_WIDTH = 640;
inline constexpr int SCREEN_HEIGHT = 480;
inline SDL_Rect* RESOLUTION = new SDL_Rect {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

inline SDL_Window* window;
inline SDL_Renderer* renderer;

inline constexpr bool fullscreen_mode = false;

inline constexpr int LETTER_WIDTH = 8;
inline constexpr int LETTER_HEIGHT = 12;
inline constexpr int LETTERS_PER_FONT_ROW = 24;

enum GameState {
    FieldFree,
    FieldUI,
    TextInput,
};
inline GameState gameState;

#endif
