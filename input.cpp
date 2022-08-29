#include <SDL2/SDL_image.h>
#include "./Input.h"
#include <iostream>

KeyPresses& Input::getInput () {
    SDL_Event input;
    keysDown.cancel = false;
    keysDown.start = false;
    keysDown.ok = false;
    keysDown.menu1 = false;
    keysDown.menu2 = false;
    while (SDL_PollEvent(&input)){
        if (input.type == SDL_QUIT){
            keysDown.quit = true;
        }
        if (input.type == SDL_KEYDOWN){
            switch( input.key.keysym.sym ){
                case SDLK_a:
                    if (input.key.repeat == 0)
                        keysDown.left = true;
                    break;
                case SDLK_d:
                    if (input.key.repeat == 0)
                        keysDown.right = true;
                    break;
                case SDLK_w:
                    if (input.key.repeat == 0)
                        keysDown.up = true;
                    break;
                case SDLK_s:
                    if (input.key.repeat == 0)
                        keysDown.down = true;
                    break;
                case SDLK_UP:
                    if (input.key.repeat == 0)
                        keysDown.ok = true;
                    break;
                case SDLK_DOWN:
                    if (input.key.repeat == 0)
                        keysDown.cancel = true;
                    break;
                case SDLK_k:
                    if (input.key.repeat == 0)
                        keysDown.menu1 = true;
                    break;
                case SDLK_j:
                    if (input.key.repeat == 0)
                        keysDown.menu2 = true;
                    break;
            }
        }
        if (input.type == SDL_KEYUP ){
            switch( input.key.keysym.sym ){
                case SDLK_a:
                    keysDown.left = false;
                    break;
                case SDLK_d:
                    keysDown.right = false;
                    break;
                case SDLK_w:
                    keysDown.up = false;
                    break;
                case SDLK_s:
                    keysDown.down = false;
                    break;
            }
        }
    }
    return keysDown;
}