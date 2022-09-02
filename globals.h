#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SDL2/SDL.h>

inline char* BASE_PATH = SDL_GetBasePath();
inline constexpr int SCALE = 2;
inline constexpr int SCREEN_WIDTH = 1200;
inline constexpr int SCREEN_HEIGHT = 800;
inline SDL_Rect* FULL_SCREEN = new SDL_Rect {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
inline int frameCount = 0;

#endif