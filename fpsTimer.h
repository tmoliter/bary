#include <SDL2/SDL.h>

class FpsTimer {
    private:
        Uint64 start;
    public:
        FpsTimer() : start(0) {};
        void startFrame();
        void endFrameAndWait(int &frameCount);
};

void FpsTimer::startFrame() {
    start = SDL_GetPerformanceCounter();
}

void FpsTimer::endFrameAndWait(int &frameCount) {
    frameCount++;
    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(16.666f - elapsedMS));
}