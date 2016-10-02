// EPQ Project
#pragma once

#include <cassert>
#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define RAD2DEG 57.2957795f
#define DEG2RAD 0.0174532925f

extern sf::Clock gTimer;
extern list<sf::Drawable*> gDrawables;

// Drawable functions
inline void addDrawable( sf::Drawable& drawable )
{
	gDrawables.push_back( &drawable );
}

inline void removeDrawable( sf::Drawable& drawable )
{
	gDrawables.remove( &drawable );
}

// Vector functions
inline float getDotProduct( const sf::Vector2f& a, const sf::Vector2f& b )
{
	return a.x * b.x + a.y * b.y;
}

inline float getLengthSq( const sf::Vector2f& v )
{
	return v.x * v.x + v.y * v.y;
}

inline float getLength( const sf::Vector2f& v )
{
	return sqrt( getLengthSq( v ) );
}

inline float getDistanceSq( const sf::Vector2f& a, const sf::Vector2f& b )
{
	sf::Vector2f delta = b - a;
	return getLengthSq( delta );
}

inline float getDistance( const sf::Vector2f& a, const sf::Vector2f& b )
{
	return sqrt( getDistanceSq( a, b  ) );
}

inline float getDistance( const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& p )
{
	// Return minimum distance between line segment vw and point p
	const float l2 = getDistanceSq( a, b );  // i.e. |w-v|^2 -  avoid a sqrt
	if( l2 == 0.0f )
		return getDistance( p, a );   // v == w case

	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = getDotProduct( p - a, b - a ) / l2;
	if (t < 0.0f)
		return getDistance( p, a );       // Beyond the 'v' end of the segment
	else if (t > 1.0f)
		return getDistance( p, b );  // Beyond the 'w' end of the segment
	const sf::Vector2f projection = a + t * ( b - a );  // Projection falls on the segment
	return getDistance( p, projection );
}

inline float normalise( sf::Vector2f& v )
{
	float length = getLength( v );
	assert( length != 0.0f );
	v /= length;
	return length;
}

// Useful functions
template <class T>
inline T lerp( T a, T b, T s )
{
	return a + ( b - a ) * s;
}

template <class T>
inline T step( T value, T step, T direction )
{
	T result;

	if( direction > value )
	{
		result = value + step;
		if( result > direction )
			return direction;
		else
			return result;
	}
	else if( direction < value )
	{
		result = value - step;
		if( result < direction )
			return direction;
		else
			return result;
	}

	return value;
}

template <class T>
inline T clamp( T x, T a, T b )
{
    return x < a ? a : ( x > b ? b : x );
}