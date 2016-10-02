#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <cstring>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std; // :(

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

typedef sf::Vector2f Vec2f;
typedef sf::Vector2i Vec2i;
typedef sf::Vector2u Vec2u;

const int MAP_X = 12;
const int MAP_Y = 12;
const int SCALE = 4;
const int TILE_SIZE = 16 * SCALE;
const int TILE_OFFSET = TILE_SIZE / 2;
const int SCREEN_X = MAP_X * TILE_SIZE;
const int SCREEN_Y = MAP_Y * TILE_SIZE;

#include "Game.h"

// Utility functions
template <class T>
inline T step(T x, T target, T step)
{
    if (x < target)
    {
        return min(x + step, target);
    }
    if (x > target)
    {
        return max(x - step, target);
    }
    return x;
}

template <class T>
inline bool floatEq(T x, T y, T eps = std::numeric_limits<T>::epsilon())
{
    return abs(x - y) < eps;
}

template <class T>
inline T lerp(T x, T y, T t)
{
    return x + (y - x) * t;
}

template <class T>
inline T len(sf::Vector2<T> v)
{
    return (T)sqrt(v.x * v.x + v.y * v.y);
}

#endif /* COMMON_H */
