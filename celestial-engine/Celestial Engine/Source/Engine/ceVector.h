/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// 2D Vector template class
template <class T> class Vector2
{
public:
	// Constructors
	Vector2()
	{
		x = y = (T)0;
	}

	Vector2( T val )
	{
		x = y = val;
	}

	Vector2( T _x, T _y )
	{
		x = _x;
		y = _y;
	}
	
	Vector2( const Vector2<T> &other )
	{
		x = other.x;
		y = other.y;
	}

	// Operators
	Vector2<T> operator-() const { return Vector2<T>( -x, -y ); }

	Vector2<T> &operator=( const Vector2<T> &other ) { x = other.x; y = other.y; return *this; }

	Vector2<T> operator+( const Vector2<T> &other ) const { return Vector2<T>( x + other.x, y + other.y ); }
	Vector2<T> &operator+=( const Vector2<T> &other ) { x += other.x; y += other.y; return *this; }
	Vector2<T> operator+( const T v ) const { return Vector2<T>( x + v, y + v ); }
	Vector2<T> &operator+=( const T v ) { x += v; y += v; return *this; }

	Vector2<T> operator-( const Vector2<T> &other ) const { return Vector2<T>( x - other.x, y - other.y ); }
	Vector2<T> &operator-=( const Vector2<T> &other ) { x -= other.x; y -= other.y; return *this; }
	Vector2<T> operator-( const T v ) const { return Vector2<T>( x - v, y - v ); }
	Vector2<T> &operator-=( const T v ) { x -= v; y -= v; return *this; }
	
	Vector2<T> operator*( const Vector2<T> &other ) const { return Vector2<T>( x * other.x, y * other.y ); }
	Vector2<T> &operator*=( const Vector2<T> &other ) { x *= other.x; y *= other.y; return *this; }
	Vector2<T> operator*( const T v ) const { return Vector2<T>( x * v, y * v ); }
	Vector2<T> &operator*=( const T v ) { x *= v; y *= v; return *this; }

	Vector2<T> operator/( const Vector2<T> &other ) const { return Vector2<T>( x / other.x, y / other.y ); }
	Vector2<T> &operator/=( const Vector2<T> &other ) { x /= other.x; y /= other.y; return *this; }
	Vector2<T> operator/( const T v ) const { return Vector2<T>( x / v, y / v ); }
	Vector2<T> &operator/=( const T v ) { x /= v; y /= v; return *this; }

	bool operator==( const Vector2<T> &other ) const { return ( x == other.x && y == other.y ); }
	bool operator!=( const Vector2<T> &other ) const { return !( x == other.x && y == other.y ); }

	// Methods
	T getLength() { return (T)sqrt( x * x + y * y ); }
	T getLengthSq() { return x * x + y * y; }
	T getDotProduct( const Vector2<T> &other ) { return x * other.x + y * other.y; }
	T getDistanceFrom( const Vector2<T> &other ) { return Vector2<T>( x - other.x, y - other.y ).getLength(); }
	T getDistanceFromSq( const Vector2<T> &other ) { return Vector2<T>( x - other.x, y - other.y ).getLengthSq(); }
	Vector2<T> normalise() { T length = getLength(); x /= length; y /= length; return *this; }
	void setLength( T length ) { normalise(); *this *= length; }
	void incert() { x = -x; y = -y; }

	// 0 is to the right (3 o'clock), values increase counter-clockwise.
	f64 getAngleTrig() const
	{
		if( y == 0.0 )
			return x < 0.0 ? 180.0 : 0.0;
		else if( x == 0.0 )
			return y < 0.0 ? 270.0 : 90.0;

		if( y > 0.0 )
		{
			if( x > 0.0 )
				return math::invtan( y / x ) * math::radToDeg64;
			else
				return 180.0 - math::invtan( y / -x ) * math::radToDeg64;
		}
		else
		{
			if( x > 0.0 )
				return 360.0 - math::invtan( -y / x ) * math::radToDeg64;
			else
				return 180.0 + math::invtan( -y / -x ) * math::radToDeg64;
		}
	}

	// 0 is to the right (3 o'clock), values increase clockwise.
	inline f64 getAngle() const
	{
		if( y == 0.0 )
			return x < 0.0 ? 180.0 : 0.0;
		else if( x == 0.0 )
			return y < 0.0 ? 90.0 : 270.0;

		// don't use getLength here to avoid precision loss with s32 Vectors
		f64 tmp = y / math::sqrt( (f64)( x * x + y * y ) );
		tmp = math::invtan( math::sqrt( 1.0 - tmp * tmp ) / tmp ) * math::radToDeg64;

		if( x > 0.0 && y > 0.0 )
			return tmp + 270.0;
		else
		if( x > 0.0 && y < 0.0 )
			return tmp + 90.0;
		else
		if( x < 0.0 && y < 0.0 )
			return 90.0 - tmp;
		else
		if( x < 0.0 && y > 0.0 )
			return 270.0 - tmp;

		return tmp;
	}

	// Members
	T x, y;
};

// 3D Vector template class
template <class T> class Vector3
{
public:
	// Constructors
	Vector3()
	{
		x = y = z = (T)0;
	}

	Vector3( T val )
	{
		x = y = z = val;
	}

	Vector3( T _x, T _y, T _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
	Vector3( const Vector3<T> &other )
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	// Operators
	Vector3<T> operator-() const { return Vector3<T>( -x, -y, -z ); }

	Vector3<T> &operator=( const Vector3<T> &other ) { x = other.x; y = other.y; z = other.z; return *this; }

	Vector3<T> operator+( const Vector3<T> &other ) const { return Vector3<T>( x + other.x, y + other.y, z + other.z ); }
	Vector3<T> &operator+=( const Vector3<T> &other ) { x += other.x; y += other.y; z += other.z; return *this; }
	Vector3<T> operator+( const T v ) const { return Vector3<T>( x + val, y + val, z + val); }
	Vector3<T> &operator+=( const T v ) { x += val; y += val; z += val; return *this; }

	Vector3<T> operator-( const Vector3<T> &other ) const { return Vector3<T>( x - other.x, y - other.y, z - other.z ); }
	Vector3<T> &operator-=( const Vector3<T> &other ) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	Vector3<T> operator-( const T v ) const { return Vector3<T>( x - val, y - val, z - val); }
	Vector3<T> &operator-=( const T v ) { x -= val; y -= val; z -= val; return *this; }

	Vector3<T> operator*( const Vector3<T> &other ) const { return Vector3<T>( x * other.x, y * other.y, z * other.z ); }
	Vector3<T> &operator*=( const Vector3<T> &other ) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	Vector3<T> operator*( const T v ) const { return Vector3<T>( x * v, y * v, z * v ); }
	Vector3<T> &operator*=( const T v ) { x *= v; y *= v; z *= v; return *this; }

	Vector3<T> operator/( const Vector3<T> &other ) const { return Vector3<T>( x / other.x, y / other.y, z / other.z ); }
	Vector3<T> &operator/=( const Vector3<T> &other ) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	Vector3<T> operator/( const T v ) const { T i = (T)1.0 / v; return Vector3<T>( x * i, y * i, z * i ); }
	Vector3<T> &operator/=( const T v ) { T i = (T)1.0 / v; x *= i; y *= i; z *= i; return *this; }
	
	bool operator==(const Vector3<T> &other) const { return ( x == other.x && y == other.y && z == other.z ); }
	bool operator!=(const Vector3<T> &other) const { return !( x == other.x && y == other.y && z == other.z ); }

	// Methods
	T getLength() { return math::sqrt<T>( x * x + y * y + z * z ); }
	T getLengthSq() { return x * x + y * y + z * z; }
	T getDotProduct( const Vector3<T> &other ) { return x * other.x + y * other.y + z * other.z; }
	Vector3<T> getCrossProduct( const Vector3<T> &other ) { return Vector3<T>( y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x ); }
	T getDistanceFrom( const Vector3<T> &other ) { return Vector3<T>( x - other.x, y - other.y, z - other.z ).getLength(); }
	T getDistanceFromSq( const Vector3<T> &other ) { return Vector3<T>( x - other.x, y - other.y, z - other.z ).getLengthSq(); }
	Vector3<T> normalise() { T length = getLength(); x /= length; y /= length; z /= length; return *this; }
	void setLength( T length ) { normalise(); *this *= length; }
	void incert() { x = -x; y = -y; z = -z; }
	
	// Get the xy euler angle in degrees necessary to point an object in the same direction as this Vector
	Vector3<T> getEulerAngle() const
	{
		Vector3<T> angle;
		
		const f64 tmp = math::invtan2( (f64)x, (f64)z ) * math::radToDeg64;
		angle.y = (T)tmp;

		if( angle.y < 0 )
			angle.y += 360;
		if( angle.y >= 360 )
			angle.y -= 360;

		const f64 z1 = math::sqrt<T>( x * x + z * z );

		angle.x = (T)( math::invtan2( (f64)z1, (f64)y ) * math::radToDeg64 - 90.0 );
		
		if( angle.x < 0 )
			angle.x += 360;
		if( angle.x >= 360 )
			angle.x -= 360;

		return angle;
	}

	// Members
	T x, y, z;
};

// Specialization for integer vector3s
template <> inline Vector3<s32> Vector3<s32>::operator/( const s32 v ) const { return Vector3<s32>( x / v, y / v, z / v ); }
template <> inline Vector3<s32>& Vector3<s32>::operator/=( const s32 v ) { x /= v; y /= v; z /= v; return *this; }

// Common Typedefs
typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector2<s32> Vector2i;
typedef Vector3<s32> Vector3i;

ceEND
