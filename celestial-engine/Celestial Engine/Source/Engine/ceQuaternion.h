/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// Simple Quaternion - Used for representing rotations
class Quaternion
{
public:
	// Constructors
	Quaternion()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	// Operators
	Quaternion &operator=( const Quaternion &q )
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quaternion operator*( const Quaternion &q )
	{
		Quaternion r;

		r.w = w * q.w - x * q.x - y * q.y - z * q.z;
		r.x = w * q.x + x * q.w + y * q.z - z * q.y;
		r.y = w * q.y + y * q.w + z * q.x - x * q.z;
		r.z = w * q.z + z * q.w + x * q.y - y * q.x;
	
		return r;
	}

	Quaternion &operator*=( const Quaternion &q )
	{
		*this = *this * q;
		return *this;
	}

	// Members
	f32 x, y, z, w;

};

// Stream operators
ostream &operator<<( ostream &os, const Quaternion &q );

// Quaternion functions
Quaternion QuatIdentity();
Quaternion QuatRotateAxis( Vector3f axis, f32 angle );

ceEND
