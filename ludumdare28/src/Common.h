// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Common_h__
#define Common_h__

#define EPSILON 1e-6f

// Standard Library
#include <assert.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
using namespace std;

// SFML
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "Collision.h"

// json-cpp
#include "json/json.h"

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// Common typedefs
typedef unsigned int uint;
typedef unsigned long ulong;
typedef sf::Vector2f Vec2;

// Math Constants
#define PI 3.14159f
#define TWO_PI (2.0f * PI)
#define RAD_TO_DEG (180.0f / PI)
#define DEG_TO_RAD (PI / 180.0f)

// Random numbers
void randomSeed(uint seed);
float random(float min, float max);
int random(int min, int max);

// Global Classes
#include "Game.h"
#include "ResourceCache.h"

// Useful functions
inline float lerp(float x, float y, float t)
{
    return x * (1.0f - t) + y * t;
}

inline float step(float x, float t, float s)
{
    if (t > x)
    {
        x += s;
        return t > x ? x : t;
    }
    if (t < x)
    {
        x -= s;
        return t < x ? x : t;
    }
    return x;
}

// Desc structures
struct EnemyPartDesc
{
    string sprite;
    uint health;
    float bulletSpeed;
    float bulletInterval;
    Vec2 position;
    float rotation;
    bool fixed;
    bool lockable;
};

struct EnemyDescTimelineEvent
{
    float time;
    Vec2 speed;
};

struct EnemyDesc
{
    string name;
    string sprite;
    uint health;
    Vec2 speed;
    list<EnemyDescTimelineEvent> timeline;
    vector<EnemyPartDesc> parts;
};

#endif    // Common_h__