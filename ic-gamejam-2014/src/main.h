#pragma once

// Standard Library
#include <math.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::vector;
using std::min;
using std::max;

// SFML
#include <SFML/Graphics.hpp>

// Box2D
#include "Box2D/Box2D.h"

// Common
#include "game.h"
#include "timer.h"

// Special Constants
#define EPSILON (1e-6f)

// Typedefs
typedef sf::Vector2f Vec2;
typedef int s32;
typedef unsigned int uint, u32;
typedef long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

// Box2D <-> SFML Inter-op
inline Vec2 fromBox(const b2Vec2& vec)
{
    return Vec2(vec.x, vec.y);
}

inline b2Vec2 toBox(const Vec2& vec)
{
    return b2Vec2(vec.x, vec.y);
}

// Lerp
inline float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

inline float lerp(float a, float b, float t, float dt)
{
    return lerp(a, b, 1.0f - pow(t, dt));
}
