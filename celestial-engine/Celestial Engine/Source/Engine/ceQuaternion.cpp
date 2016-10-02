/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"

ceBEGIN
	
// Stream operators
ostream &operator<<( ostream &os, const Quaternion &q )
{
	os << q.x << ", " << q.y << ", " << q.z << ", " << q.w;
	return os;
}

// Quaternion functions
Quaternion QuatIdentity()
{
	Quaternion out;
	return out;
}

Quaternion QuatRotateAxis( Vector3f axis, f32 angle )
{
	Quaternion out;

	f32 result = math::sin( angle / 2.0f );
	out.x = axis.x * result;
	out.y = axis.y * result;
	out.z = axis.z * result;
	out.w = math::cos( angle / 2.0f );

	return out;
}

ceEND
