#include "barysystem.h"

void barysystem::startup(vector<string>& saveNames) {
    settings.init();
    char* basePath = SDL_GetBasePath();
    if (basePath) {
        if (0 == cd(basePath)) {
            std::cout << "CWD changed successfully" << std::endl;
        }
    } else {
        std::cout << "Couldn't find application directory" << std::endl;
    }

    const regex base_regex(R"(^.*\/([^\/]+)\.lua)");
    string basePathString(basePath);
    string savePath = basePathString + "/" + settings.GAME_NAME + "/saves";

    smatch base_match;
    for (const auto & entry : fs::directory_iterator(savePath)) {
        string path(entry.path());
        if (regex_match(path, base_match, base_regex) && base_match.size() == 2)
                saveNames.push_back(base_match[1].str());
    }


    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // audio stuff is commented out because it fucks with the laptop sound after debugging

    // if( Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0 )
    // {
    //     fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    //     exit(-1);
    // }

    // if( Mix_AllocateChannels(4) < 0 )
    // {
    //     fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
    //     exit(-1);
    // }

    SDL_Window* window = SDL_CreateWindow(
        "Bary",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        settings.FULLSCREEN_MODE ? 0 : settings.SCREEN_WIDTH, settings.FULLSCREEN_MODE ? 0 :settings.SCREEN_HEIGHT,
        settings.FULLSCREEN_MODE ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN
    );

    // Play with third arguemnt?
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (settings.FULLSCREEN_MODE) {
        SDL_RenderSetLogicalSize(renderer, settings.SCREEN_WIDTH, settings.SCREEN_HEIGHT);
        SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

