#include <SDL2/SDL.h>

class fpsTimer {
    private:
        Uint64 start;

    public:
        fpsTimer() : start(0) {};
        void startFrame();
        void endFrameAndWait(int &frameCount);
};

void fpsTimer::startFrame() {
    start = SDL_GetPerformanceCounter();
}

void fpsTimer::endFrameAndWait(int &frameCount) {
    frameCount++;
    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(16.666f - elapsedMS));
}