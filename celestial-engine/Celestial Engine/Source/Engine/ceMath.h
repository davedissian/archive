/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

#include <math.h>

ceBEGIN

namespace math
{

// Constants
const f32 pi = 3.14159265359f;
const f32 reciprocalPi = 1.0f / pi;
const f64 pi64 = 3.1415926535897932384626433832795028841971693993751;
const f64 reciprocalPi64 = 1.0 / pi64;

const f32 radToDeg = 180.0f / pi;
const f64 radToDeg64 = 180.0 / pi64;
const f32 degToRad = pi / 180.0f;
const f64 degToRad64 = pi64 / 180.0;

// Methods
template <class T, class P>
inline T power( T val, P power )
{
	return (T)::pow( (f64)val, (f64)power );
}

template <>
inline f32 power( f32 val, f32 power )
{
	return ::powf( val, power );
}

template <>
inline f64 power( f64 val, f64 power )
{
	return ::pow( val, power );
}

template <class T>
inline T sqrt( T val )
{
	return (T)::sqrt( (f64)val );
}

template <>
inline f32 sqrt( f32 val )
{
	return ::sqrtf( val );
}

template <>
inline f64 sqrt( f64 val )
{
	return ::sqrt( val );
}

inline f32 sin( f32 val )
{
	return ::sinf( val );
}

inline f64 sin( f64 val )
{
	return ::sin( val );
}

inline f32 cos( f32 val )
{
	return ::cosf( val );
}

inline f64 cos( f64 val )
{
	return ::cos( val );
}

inline f32 tan( f32 val )
{
	return ::tanf( val );
}

inline f64 tan( f64 val )
{
	return ::tan( val );
}

inline f32 invsin( f32 val )
{
	return ::asinf( val );
}

inline f64 invsin( f64 val )
{
	return ::asin( val );
}

inline f32 invcos( f32 val )
{
	return ::acosf( val );
}

inline f64 invcos( f64 val )
{
	return ::acos( val );
}

inline f32 invtan( f32 val )
{
	return ::atanf( val );
}

inline f64 invtan( f64 val )
{
	return ::atan( val );
}

inline f32 invtan2( f32 y, f32 x )
{
	return ::atan2( y, x );
}

inline f64 invtan2( f64 y, f64 x )
{
	return ::atan2( y, x );
}

inline f32 cot( f32 val )
{
	return 1.0f / ::tanf( val );
}

inline f64 cot( f64 val )
{
	return 1.0 / ::tan( val );
}

inline f32 cosec( f32 val )
{
	return 1.0f / ::sinf( val );
}

inline f64 cosec( f64 val )
{
	return 1.0 / ::sin( val );
}

inline f32 sec( f32 val )
{
	return 1.0f / ::cosf( val );
}

inline f64 sec( f64 val )
{
	return 1.0f / ::cos( val );
}

template <class T>
inline T abs( T val )
{
	if( val < 0 )
		return -val;
	else
		return val;
}

inline f32 round( f32 val )
{
	f32 decimal = val - ::floorf( val );
	if( decimal >= 0.5f )
		return ::floorf( val ) + 1.0f;
	else
		return ::floorf( val );
}

inline f64 round( f64 val )
{
	f64 decimal = val - ::floor( val );
	if( decimal >= 0.5 )
		return ::floor( val ) + 1.0;
	else
		return ::floor( val );
}

inline f32 reciprocal( f32 val )
{
	return 1.0f / val;
}

inline f64 reciprocal( f64 val )
{
	return 1.0 / val;
}

}

ceEND
