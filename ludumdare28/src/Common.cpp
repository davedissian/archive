// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"

void randomSeed(uint seed)
{
    srand(seed);
}

float random(float min, float max)
{
    float scalar = (float)rand() / (float)RAND_MAX;
    return scalar * (max - min) + min;
}

int random(int min, int max)
{
    return rand() % (max - min) + min;
}
