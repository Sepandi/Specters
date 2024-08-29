#define STB_PERLIN_IMPLEMENTATION
#include "random.h"

unsigned int GetRandomSeed() {
    unsigned int seed;
    
    // Use platform-specific methods to generate a seed
    #ifdef _WIN32
        // Windows: Use QueryPerformanceCounter if available, otherwise use time and Sleep
        LARGE_INTEGER performance_count;
        if (QueryPerformanceCounter(&performance_count)) {
            seed = performance_count.LowPart ^ GetCurrentProcessId();
        } else {
            seed = time(NULL) ^ GetCurrentProcessId() ^ GetCurrentThreadId() ^ GetTickCount();
        }
    #else
        // Unix-like systems: Use getpid, time, and usleep
        seed = getpid() ^ (unsigned int)time(NULL);
        usleep(100); // Add a small delay to ensure different seeds in fast processes
    #endif
    
    return 0;
}

float GetPerlinValue(float x, float y,int seed){
    return (stb_perlin_noise3_seed(x,y,0,0,0,0,seed)+1)/2;
}

int RandInt(int minRange, int maxRange) {
    // Seed the random number generator
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand(time(NULL));
        seed_initialized = 1;
    }

    // Generate a random number within the specified range
    return minRange + rand() % (maxRange - minRange + 1);
}