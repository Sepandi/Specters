#pragma once

#include "house.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#ifdef _WIN32
    #include "windows.h"
#endif 
#ifdef __APPLE__
    #include "unistd.h"
#endif


#include "sep/stb/stb_perlin.h"

unsigned int GetRandomSeed();

float GetPerlinValue(float x, float y,int seed);

int RandInt(int minRange, int maxRange);