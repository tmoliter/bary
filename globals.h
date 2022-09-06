#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL2/SDL.h>

inline char* BASE_PATH = SDL_GetBasePath();
inline constexpr int SCALE = 1;
inline constexpr int SCREEN_WIDTH = 1280;
inline constexpr int SCREEN_HEIGHT = 960;
inline SDL_Rect* FULL_SCREEN = new SDL_Rect {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
inline int frameCount = 0;
inline SDL_Window* window;
inline SDL_Renderer* renderer;

inline constexpr bool fullscreen_mode = false;

#endif