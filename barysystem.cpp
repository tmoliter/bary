#include "barysystem.h"

void barysystem::startup(vector<string>& saveNames) {
    settings.init();

    string savePath = settings.BASE_PATH + "games/" + settings.GAME_NAME + "/saves";
    smatch base_match;
    const regex base_regex(R"(^.*\/([^\/]+)\.lua)");
    for (const auto & entry : fs::directory_iterator(savePath)) {
        string path(entry.path());
        if (regex_match(path, base_match, base_regex) && base_match.size() == 2)
                saveNames.push_back(base_match[1].str());
    }


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

    window = SDL_CreateWindow(
        "Bary",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        settings.FULLSCREEN_MODE ? 0 : settings.SCREEN_WIDTH, settings.FULLSCREEN_MODE ? 0 :settings.SCREEN_HEIGHT,
        settings.FULLSCREEN_MODE ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN
    );

    // Play with third argument?
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (settings.FULLSCREEN_MODE) {
        SDL_RenderSetLogicalSize(renderer, settings.SCREEN_WIDTH, settings.SCREEN_HEIGHT);
        SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

string barysystem::nextSaveName() {
    string savePath = settings.BASE_PATH + "games/" + settings.GAME_NAME + "/saves";
    const regex saveRegex(R"(^Save (\d+)$)");
    smatch match;
    int maxN = 0;
    for (const auto & entry : fs::directory_iterator(savePath)) {
        string stem = entry.path().stem().string();   // "Save 3" from "Save 3.lua"
        if (regex_match(stem, match, saveRegex))
            maxN = max(maxN, stoi(match[1].str()));
    }
    return "Save " + to_string(maxN + 1);
}

