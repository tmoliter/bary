#include "./Input.h"

using namespace std;

KeyPresses& Input::getInput () {
    SDL_Event input;
    keysDown.ok = false;
    keysDown.cancel = false;
    keysDown.menu1 = false;
    keysDown.menu2 = false;
    keysDown.start = false;
    keysDown.nav_up = false;
    keysDown.nav_down = false;
    keysDown.nav_left = false;
    keysDown.nav_right = false;
    
    keysDown.textInput = false;
    keysDown.del = false;

    keysDown.debug_plus = false;
    keysDown.debug_minus = false;
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
    while (SDL_PollEvent(&input)){
        if (input.type == SDL_QUIT)
            keysDown.quit = true;
        if (gameState == GameState::TextInput) {
            if(input.type == SDL_KEYDOWN) {
                SDL_Keycode c = input.key.keysym.sym;
                if (int(c) == SDLK_RETURN)
                    keysDown.start = true;
                if (int(c) == SDLK_BACKSPACE)
                    keysDown.del = true;
                if (int(c) == SDLK_UP)
                    keysDown.debug_up = true;
                if (int(c) == SDLK_DOWN)
                    keysDown.debug_down = true;
            }
            else if (input.type == SDL_TEXTINPUT) {
                char c = input.text.text[0];
                if(c >= ' ' || c <= '~')
					keysDown.textInput = c;
            }
            return keysDown;
        }
        if (input.type == SDL_KEYDOWN){
            switch( input.key.keysym.sym ){
                case SDLK_q:
                    if (input.key.repeat == 0)
                        keysDown.quit = true;
                    break;
                case SDLK_a:
                    if (input.key.repeat == 0) {
                        keysDown.left = true;
                        keysDown.nav_left = true;
                    }
                    break;
                case SDLK_d:
                    if (input.key.repeat == 0) {
                        keysDown.right = true;
                        keysDown.nav_right = true;
                    }
                    break;
                case SDLK_w:
                    if (input.key.repeat == 0) {
                        keysDown.up = true;
                        keysDown.nav_up = true;
                    }
                    break;
                case SDLK_s:
                    if (input.key.repeat == 0) {
                        keysDown.down = true;
                        keysDown.nav_down = true;
                    }
                    break;
                case SDLK_j:
                    if (input.key.repeat == 0)
                        keysDown.cancel = true;
                    break;
                case SDLK_k:
                    if (input.key.repeat == 0)
                        keysDown.ok = true;
                    break;
                case SDLK_u:
                    if (input.key.repeat == 0)
                        keysDown.menu1 = true;
                    break;
                case SDLK_i:
                    if (input.key.repeat == 0)
                        keysDown.menu2 = true;
                    break;
                case SDLK_RETURN:
                    if (input.key.repeat == 0)
                        keysDown.start = true;
                    break;
                    
                /* DEBUG */
                case SDLK_EQUALS:
                    if (input.key.repeat == 0)
                        keysDown.debug_plus = true;
                    break;
                case SDLK_MINUS:
                    if (input.key.repeat == 0)
                        keysDown.debug_minus = true;
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