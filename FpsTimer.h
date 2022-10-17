#ifndef FPS_TIMER_H
#define FPS_TIMER_H

#include <SDL2/SDL.h>
#include <iostream>

struct ProfileData {
    float a;
    float b;
    float c;
    float d;
    float e;
};

class FpsTimer {
    private:
        Uint64 start;
    public:
        FpsTimer() : start(0) {};
        void startFrame();
        void timeElapsed(float *time);
        void timeElapsed(float *time, float last);
        void endFrameAndWait(int &frameCount);
        void endFrameAndWait(int &frameCount, ProfileData &profileData);
};

void FpsTimer::startFrame() {
    start = SDL_GetPerformanceCounter();
}

void FpsTimer::timeElapsed(float *time) {
    Uint64 now = SDL_GetPerformanceCounter();
    *time = (now - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
}

void FpsTimer::endFrameAndWait(int &frameCount) {
    frameCount++;
    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(floor(16.666f - elapsedMS));
}


void FpsTimer::endFrameAndWait(int &frameCount, ProfileData &profileData) {
    frameCount++;
    Uint64 end = SDL_GetPerformanceCounter();
    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
    if (elapsedMS > 20) {
        std::cout << "a: " << profileData.a << std::endl;
        std::cout << "b: " << profileData.b << std::endl;
        std::cout << "c: " << profileData.c << std::endl;
        std::cout << "d: " << profileData.d << std::endl;
        std::cout << "e: " << profileData.e << std::endl;
        std::cout << "total: " << elapsedMS << std::endl <<std::endl;
    }
    SDL_Delay(floor(16.666f - elapsedMS));
}

#endif
