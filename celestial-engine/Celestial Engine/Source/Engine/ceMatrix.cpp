/*
	Celestial Engine
	Created by SerialVelocity
*/
#include "celestialBase.h"

ceBEGIN
	
// Stream operators
ostream &operator<<( ostream &os, const Matrix &matrix )
{
	for( u32 i = 0; i < 4; ++i )
	{
		for( u32 j = 0; j < 4; ++j )
		{
			os << matrix.m[i][j];
			if( j < 3 )
				os << ", ";
		}
		if( i < 3 )
			os << endl;
	}
	return os;
}

// Matrix functions
Matrix MatIdentity()
{
	Matrix out;

	for( u32 i = 0; i < 4; ++i )
	{
		for( u32 j = 0; j < 4; ++j )
		{
			if( i == j )
				out.m[i][j] = 1.0f;
			else
				out.m[i][j] = 0.0f;
		}
	}

	return out;
}

Matrix MatTranspose( const Matrix &mat ) // Convert from row major to column major and vice versa
{
	Matrix out;

	for( u32 i = 0; i < 4; ++i )
	{
		for( u32 j = 0; j < 4; ++j )
			out.m[i][j] = mat.m[j][i];
	}

	return out;
}

Matrix MatTranslate( Vector3f translation )
{
	Matrix out = MatIdentity();

	out.m30 = translation.x;
	out.m31 = translation.y;
	out.m32 = translation.z;

	return out;
}

Matrix MatRotate( const Quaternion &q )
{
	Matrix out = MatIdentity();

	out.m00 = 1.0f - 2.0f * ( q.y * q.y + q.z * q.z );
	out.m01 = 2.0f * ( q.x * q.y - q.w * q.z );
	out.m02 = 2.0f * ( q.x * q.z + q.w * q.y );
	out.m03 = 0.0f;
	out.m10 = 2.0f * ( q.x * q.y + q.w * q.z );
	out.m11 = 1.0f - 2.0f * ( q.x * q.x + q.z * q.z );
	out.m12 = 2.0f * ( q.y * q.z - q.w * q.x );
	out.m13 = 0.0f;
	out.m20 = 2.0f * ( q.x * q.z - q.w * q.y );
	out.m21 = 2.0f * ( q.y * q.z + q.w * q.x );
	out.m22 = 1.0f - 2.0f * ( q.x * q.x + q.y * q.y );
	out.m23 = 0.0f;

	return out;
}

Matrix MatScale( Vector3f scale )
{
	Matrix out = MatIdentity();

	out.m00 = scale.x;
	out.m11 = scale.y;
	out.m22 = scale.z;

	return out;
}

f32 MatGetDeterminant( const Matrix &mat )
{
    return mat.m03 * mat.m12 * mat.m21 * mat.m30 - mat.m02 * mat.m13 * mat.m21 * mat.m30 - mat.m03 * mat.m11 * mat.m22 * mat.m30 + mat.m01 * mat.m13 * mat.m22 * mat.m30 + mat.m02 * mat.m11 * mat.m23 * mat.m30 - mat.m01 * mat.m12 * mat.m23 * mat.m30 - mat.m03 * mat.m12 * mat.m20 * mat.m31 + mat.m02 * mat.m13 * mat.m20 * mat.m31 + mat.m03 * mat.m10 * mat.m22 * mat.m31 - mat.m00 * mat.m13 * mat.m22 * mat.m31 - mat.m02 * mat.m10 * mat.m23 * mat.m31 + mat.m00 * mat.m12 * mat.m23 * mat.m31 + mat.m03 * mat.m11 * mat.m20 * mat.m32 - mat.m01 * mat.m13 * mat.m20 * mat.m32 - mat.m03 * mat.m10 * mat.m21 * mat.m32 + mat.m00 * mat.m13 * mat.m21 * mat.m32 + mat.m01 * mat.m10 * mat.m23 * mat.m32 - mat.m00 * mat.m11 * mat.m23 * mat.m32 - mat.m02 * mat.m11 * mat.m20 * mat.m33 + mat.m01 * mat.m12 * mat.m20 * mat.m33 + mat.m02 * mat.m10 * mat.m21 * mat.m33 - mat.m00 * mat.m12 * mat.m21 * mat.m33 - mat.m01 * mat.m10 * mat.m22 * mat.m33 + mat.m00 * mat.m11 * mat.m22 * mat.m33;
}

Matrix MatInverse( const Matrix &mat )
{
	Matrix out;
	out.m00 = mat.m12 * mat.m23 * mat.m31 - mat.m13 * mat.m22 * mat.m31 + mat.m13 * mat.m21 * mat.m32 - mat.m11 * mat.m23 * mat.m32 - mat.m12 * mat.m21 * mat.m33 + mat.m11 * mat.m22 * mat.m33;
	out.m01 = mat.m03 * mat.m22 * mat.m31 - mat.m02 * mat.m23 * mat.m31 - mat.m03 * mat.m21 * mat.m32 + mat.m01 * mat.m23 * mat.m32 + mat.m02 * mat.m21 * mat.m33 - mat.m01 * mat.m22 * mat.m33;
	out.m02 = mat.m02 * mat.m13 * mat.m31 - mat.m03 * mat.m12 * mat.m31 + mat.m03 * mat.m11 * mat.m32 - mat.m01 * mat.m13 * mat.m32 - mat.m02 * mat.m11 * mat.m33 + mat.m01 * mat.m12 * mat.m33;
	out.m03 = mat.m03 * mat.m12 * mat.m21 - mat.m02 * mat.m13 * mat.m21 - mat.m03 * mat.m11 * mat.m22 + mat.m01 * mat.m13 * mat.m22 + mat.m02 * mat.m11 * mat.m23 - mat.m01 * mat.m12 * mat.m23;
	out.m10 = mat.m13 * mat.m22 * mat.m30 - mat.m12 * mat.m23 * mat.m30 - mat.m13 * mat.m20 * mat.m32 + mat.m10 * mat.m23 * mat.m32 + mat.m12 * mat.m20 * mat.m33 - mat.m10 * mat.m22 * mat.m33;
	out.m11 = mat.m02 * mat.m23 * mat.m30 - mat.m03 * mat.m22 * mat.m30 + mat.m03 * mat.m20 * mat.m32 - mat.m00 * mat.m23 * mat.m32 - mat.m02 * mat.m20 * mat.m33 + mat.m00 * mat.m22 * mat.m33;
	out.m12 = mat.m03 * mat.m12 * mat.m30 - mat.m02 * mat.m13 * mat.m30 - mat.m03 * mat.m10 * mat.m32 + mat.m00 * mat.m13 * mat.m32 + mat.m02 * mat.m10 * mat.m33 - mat.m00 * mat.m12 * mat.m33;
	out.m13 = mat.m02 * mat.m13 * mat.m20 - mat.m03 * mat.m12 * mat.m20 + mat.m03 * mat.m10 * mat.m22 - mat.m00 * mat.m13 * mat.m22 - mat.m02 * mat.m10 * mat.m23 + mat.m00 * mat.m12 * mat.m23;
	out.m20 = mat.m11 * mat.m23 * mat.m30 - mat.m13 * mat.m21 * mat.m30 + mat.m13 * mat.m20 * mat.m31 - mat.m10 * mat.m23 * mat.m31 - mat.m11 * mat.m20 * mat.m33 + mat.m10 * mat.m21 * mat.m33;
	out.m21 = mat.m03 * mat.m21 * mat.m30 - mat.m01 * mat.m23 * mat.m30 - mat.m03 * mat.m20 * mat.m31 + mat.m00 * mat.m23 * mat.m31 + mat.m01 * mat.m20 * mat.m33 - mat.m00 * mat.m21 * mat.m33;
	out.m22 = mat.m01 * mat.m13 * mat.m30 - mat.m03 * mat.m11 * mat.m30 + mat.m03 * mat.m10 * mat.m31 - mat.m00 * mat.m13 * mat.m31 - mat.m01 * mat.m10 * mat.m33 + mat.m00 * mat.m11 * mat.m33;
	out.m23 = mat.m03 * mat.m11 * mat.m20 - mat.m01 * mat.m13 * mat.m20 - mat.m03 * mat.m10 * mat.m21 + mat.m00 * mat.m13 * mat.m21 + mat.m01 * mat.m10 * mat.m23 - mat.m00 * mat.m11 * mat.m23;
	out.m30 = mat.m12 * mat.m21 * mat.m30 - mat.m11 * mat.m22 * mat.m30 - mat.m12 * mat.m20 * mat.m31 + mat.m10 * mat.m22 * mat.m31 + mat.m11 * mat.m20 * mat.m32 - mat.m10 * mat.m21 * mat.m32;
	out.m31 = mat.m01 * mat.m22 * mat.m30 - mat.m02 * mat.m21 * mat.m30 + mat.m02 * mat.m20 * mat.m31 - mat.m00 * mat.m22 * mat.m31 - mat.m01 * mat.m20 * mat.m32 + mat.m00 * mat.m21 * mat.m32;
	out.m32 = mat.m02 * mat.m11 * mat.m30 - mat.m01 * mat.m12 * mat.m30 - mat.m02 * mat.m10 * mat.m31 + mat.m00 * mat.m12 * mat.m31 + mat.m01 * mat.m10 * mat.m32 - mat.m00 * mat.m11 * mat.m32;
	out.m33 = mat.m01 * mat.m12 * mat.m20 - mat.m02 * mat.m11 * mat.m20 + mat.m02 * mat.m10 * mat.m21 - mat.m00 * mat.m12 * mat.m21 - mat.m01 * mat.m10 * mat.m22 + mat.m00 * mat.m11 * mat.m22;
    out *= MatScale( 1.0f / MatGetDeterminant( mat ) );
	return out;
}

ceEND
