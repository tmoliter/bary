#include <SDL2/SDL_image.h>
#include "./Input.h"
#include <iostream>

KeyPresses& Input::getInput () {
    SDL_Event input;
    keysDown.cancel = false;
    keysDown.start = false;
    keysDown.ok = false;
    keysDown.debug_up = false;
    keysDown.debug_down = false;
    keysDown.debug_left = false;
    keysDown.debug_right = false;
    keysDown.debug_1 = false;
    keysDown.debug_2 = false;
    keysDown.debug_3 = false;
    keysDown.debug_4 = false;
    keysDown.debug_5 = false;
    keysDown.debug_6 = false;
    keysDown.debug_7 = false;
    keysDown.debug_8 = false;
    keysDown.debug_9 = false;
    keysDown.debug_0 = false;
    keysDown.debug_return = false;
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
                        keysDown.debug_up = true;
                    break;
                case SDLK_DOWN:
                    if (input.key.repeat == 0)
                        keysDown.debug_down = true;
                    break;
                case SDLK_LEFT:
                    if (input.key.repeat == 0)
                        keysDown.debug_left = true;
                    break;
                case SDLK_RIGHT:
                    if (input.key.repeat == 0)
                        keysDown.debug_right = true;
                    break;
                /* DEBUG */
                case SDLK_1:
                    if (input.key.repeat == 0)
                        keysDown.debug_1 = true;
                    break;
                case SDLK_2:
                    if (input.key.repeat == 0)
                        keysDown.debug_2 = true;
                    break;
                case SDLK_3:
                    if (input.key.repeat == 0)
                        keysDown.debug_3 = true;
                    break;
                case SDLK_4:
                    if (input.key.repeat == 0)
                        keysDown.debug_4 = true;
                    break;
                case SDLK_5:
                    if (input.key.repeat == 0)
                        keysDown.debug_5 = true;
                    break;
                case SDLK_6:
                    if (input.key.repeat == 0)
                        keysDown.debug_6 = true;
                    break;
                case SDLK_7:
                    if (input.key.repeat == 0)
                        keysDown.debug_7 = true;
                    break;
                case SDLK_8:
                    if (input.key.repeat == 0)
                        keysDown.debug_8 = true;
                    break;
                case SDLK_9:
                    if (input.key.repeat == 0)
                        keysDown.debug_9 = true;
                    break;
                case SDLK_0:
                    if (input.key.repeat == 0)
                        keysDown.debug_0 = true;
                    break;
                case SDLK_RETURN:
                    if (input.key.repeat == 0)
                        keysDown.debug_return = true;
                    break;
                /* END DEBUG */
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