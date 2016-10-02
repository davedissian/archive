/*
	Maths Support Library
	By SerialVelocity
*/
#pragma once

// Support Header files
#include <iostream>

// 2D Vector template
template <class t>
class CTVector2
{
public:
	// Constructors
	CTVector2()
	{
		x = y = (t)0;
	}

	CTVector2( t _x, t _y )
	{
		x = _x;
		y = _y;
	}

	// Assignment Operators
	CTVector2 &operator = ( CTVector2 &rhs )
	{
		// Perform an assignment
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	CTVector2 &operator += ( CTVector2 &rhs )
	{
		// Perform an addition
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	CTVector2 &operator -= ( CTVector2 &rhs )
	{
		// Perform a subtraction
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	CTVector2 &operator *= ( t rhs )
	{
		// Perform a multiplication
		x *= rhs;
		y *= rhs;
		return *this;
	}

	CTVector2 &operator /= ( t rhs )
	{
		// Perform a division
		x /= rhs;
		y /= rhs;
		return *this;
	}

	// Binary Operators
	CTVector2 operator + ( CTVector2 &rhs ) const
	{
		// Perform an addition
		CTVector2<t> result = *this;
		result += rhs;
		return result;
	}

	CTVector2 operator - ( CTVector2 &rhs ) const
	{
		// Perform a subtraction
		CTVector2<t> result = *this;
		result -= rhs;
		return result;
	}

	CTVector2 operator * ( t rhs ) const
	{
		// Perform a multiplication
		CTVector2<t> result = *this;
		result *= rhs;
		return result;
	}

	CTVector2 operator / ( t rhs ) const
	{
		// Perform a division
		CTVector2<t> result = *this;
		result /= rhs;
		return result;
	}

	// Comparison Operators
	bool operator == ( CTVector2 &rhs ) const
	{
		// If all members are equal, return true, else false
		if( this->x == rhs.x && this->y == rhs.y ) return true;
		return false;
	}

	bool operator != ( CTVector2 &rhs ) const
	{
		// Inverse of the == operator
		return !( *this == rhs );
	}

	// StL Operators
	friend std::ostream& operator << ( std::ostream &out, CTVector2 &vector )
	{
		out << "[ " << vector.x << ", " << vector.y << "]";
		return out;
	}

	// Members
	t x, y;
};

// 3D Vector template
template <class t>
class CTVector3
{
public:
	// Constructors
	CTVector3()
	{
		x = y = z = (t)0;
	}

	CTVector3( t _x, t _y, t _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	// Assignment Operators
	CTVector3 &operator = ( CTVector3 &rhs )
	{
		// Perform an assignment
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	CTVector3 &operator += ( CTVector3 &rhs )
	{
		// Perform an addition
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	CTVector3 &operator -= ( CTVector3 &rhs )
	{
		// Perform a subtraction
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	CTVector3 &operator *= ( t rhs )
	{
		// Perform a multiplication
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	CTVector3 &operator /= ( t rhs )
	{
		// Perform a division
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	// Binary Operators
	CTVector3 operator + ( CTVector3 &rhs ) const
	{
		// Perform an addition
		CTVector3<t> result = *this;
		result += rhs;
		return result;
	}

	CTVector3 operator - ( CTVector3 &rhs ) const
	{
		// Perform a subtraction
		CTVector3<t> result = *this;
		result -= rhs;
		return result;
	}

	CTVector3 operator * ( t rhs ) const
	{
		// Perform a multiplication
		CTVector3<t> result = *this;
		result *= rhs;
		return result;
	}

	CTVector3 operator / ( t rhs ) const
	{
		// Perform a division
		CTVector3<t> result = *this;
		result /= rhs;
		return result;
	}

	// Comparison Operators
	bool operator == ( CTVector3 &rhs ) const
	{
		// If all members are equal, return true, else false
		if( this->x == rhs.x && this->y == rhs.y && this->z == rhs.z ) return true;
		return false;
	}

	bool operator != ( CTVector3 &rhs ) const
	{
		// Inverse of the == operator
		return !( *this == rhs );
	}

	// StL Operators
	friend std::ostream& operator << ( std::ostream &out, CTVector3 &vector )
	{
		out << "[ " << vector.x << ", " << vector.y << ", " << vector.z << "]";
		return out;
	}

	// Members
	t x, y, z;
};

// 4D Vector template
template <class t>
class CTVector4
{
public:
	// Constructors
	CTVector4()
	{
		x = y = z = w = (t)0;
	}

	CTVector4( t _x, t _y, t _z, t _w )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	// Assignment Operators
	CTVector4 &operator = ( CTVector4 &rhs )
	{
		// Perform an assignment
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	CTVector4 &operator += ( CTVector4 &rhs )
	{
		// Perform an addition
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	CTVector4 &operator -= ( CTVector4 &rhs )
	{
		// Perform a subtraction
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	CTVector4 &operator *= ( t rhs )
	{
		// Perform a multiplication
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	CTVector4 &operator /= ( t rhs )
	{
		// Perform a division
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	// Binary Operators
	CTVector4 operator + ( CTVector4 &rhs ) const
	{
		// Perform an addition
		CTVector4<t> result = *this;
		result += rhs;
		return result;
	}

	CTVector4 operator - ( CTVector4 &rhs ) const
	{
		// Perform a subtraction
		CTVector4<t> result = *this;
		result -= rhs;
		return result;
	}

	CTVector4 operator * ( t rhs ) const
	{
		// Perform a multiplication
		CTVector4<t> result = *this;
		result *= rhs;
		return result;
	}

	CTVector4 operator / ( t rhs ) const
	{
		// Perform a division
		CTVector4<t> result = *this;
		result /= rhs;
		return result;
	}

	// Comparison Operators
	bool operator == ( CTVector4 &rhs ) const
	{
		// If all members are equal, return true, else false
		if( this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w ) return true;
		return false;
	}

	bool operator != ( CTVector4 &rhs ) const
	{
		// Inverse of the == operator
		return !( *this == rhs );
	}

	// StL Operators
	friend std::ostream& operator << ( std::ostream &out, CTVector4 &vector )
	{
		out << "[ " << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << "]";
		return out;
	}

	// Members
	t x, y, z, w;
};

// nD Vector template
template <int n, class t>
class CTVectorN
{
public:
	// Constructors
	CTVectorN()
	{
		for( int i = 0; i < n; i++ )
			m[i] = (t)0;
		size = n;
	}

	CTVectorN( t v )
	{
		for( int i = 0; i < n; i++ )
			m[i] = v;
	}

	// Assignment Operators
	CTVectorN &operator = ( CTVectorN &rhs )
	{
		// Make sure this and the rhs arent the same
		if( this != &rhs )
		{
			// Perform Assignment
			for( int i = 0; i < n; i++ )
				m[i] = rhs.m[i];
		}
		return *this;
	}

	CTVectorN &operator += ( CTVectorN &rhs )
	{
		// Perform an addition
		for( int i = 0; i < n; i++ )
			m[i] += rhs.m[i];
		return *this;
	}

	CTVectorN &operator -= ( CTVectorN &rhs )
	{
		// Perform a subtraction
		for( int i = 0; i < n; i++ )
			m[i] -= rhs.m[i];
		return *this;
	}

	CTVectorN &operator *= ( t rhs )
	{
		// Perform a multiplication
		for( int i = 0; i < n; i++ )
			m[i] *= rhs;
		return *this;
	}

	CTVectorN &operator /= ( t rhs )
	{
		// Perform a division
		for( int i = 0; i < n; i++ )
			m[i] /= rhs;
		return *this;
	}

	// Binary Operators
	CTVectorN operator + ( CTVectorN &rhs ) const
	{
		// Perform an addition
		CTVectorN<n, t> result = *this;
		result += rhs;
		return result;
	}

	CTVectorN operator - ( CTVectorN &rhs ) const
	{
		// Perform a subtraction
		CTVectorN<n, t> result = *this;
		result -= rhs;
		return result;
	}

	CTVectorN operator * ( t rhs ) const
	{
		// Perform a multiplication
		CTVectorN<n, t> result = *this;
		result *= rhs;
		return result;
	}

	CTVectorN operator / ( t rhs ) const
	{
		// Perform a division
		CTVectorN<n, t> result = *this;
		result /= rhs;
		return result;
	}

	// Comparison Operators
	bool operator == ( CTVectorN &rhs ) const
	{
		// If any member doesn't match, return false
		for( int i = 0; i < n; i++ )
		{
			if( m[i] != rhs.m[i] )
				return false;
		}

		// Must be equal
		return true;
	}

	bool operator != ( CTVectorN &rhs ) const
	{
		// Inverse of the == operator
		return !( *this == rhs );
	}

	// StL Operators
	friend std::ostream &operator << ( std::ostream &out, CTVectorN &rhs )
	{
		out << "[ ";
		for( int i = 0; i < n; i++ )
		{
			out << rhs.m[i];
			if( i < n - 1 )
				out << ", ";
		}
		out << " ]";
		return out;
	}

	// Accessors
	int size()
	{
		return mSize;
	}

	// Members
	t m[n];

private:
	// Vector Size
	int mSize;
};

// Common Vector types
typedef CTVector2<int> IntVec2;
typedef CTVector3<int> IntVec3;
typedef CTVector4<int> IntVec4;
