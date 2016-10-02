/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

ceBEGIN

// 4x4 Matrix (row major)
class Matrix
{
public:
	// Constructors
	Matrix()
	{
		memset( m, 0, sizeof( f32 ) * 16 );
	}

	// Operators
	Matrix &operator=( const Matrix &other )
	{
		memcpy( m, other.m, sizeof( f32 ) * 16 );
		return *this;
	}

	Matrix &operator+=( const Matrix &other )
	{
		for( s32 i = 0; i < 4; ++i )
		{
			for( s32 j = 0; j < 4; ++j )
			{
				m[i][j] += other.m[i][j];
			}
		}

		return *this;
	}

	Matrix operator+( const Matrix &other )
	{
		Matrix out = *this;
		out += other;
		return out;
	}

	Matrix &operator-=( const Matrix &other )
	{
		for( s32 i = 0; i < 4; ++i )
		{
			for( s32 j = 0; j < 4; ++j )
			{
				m[i][j] -= other.m[i][j];
			}
		}

		return *this;
	}

	Matrix operator-( const Matrix &other )
	{
		Matrix out = *this;
		out -= other;
		return out;
	}

	Matrix operator*( const Matrix &other )
	{
		Matrix out;
		
		out.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
		out.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
		out.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
		out.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;
		out.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
		out.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
		out.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
		out.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;
		out.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
		out.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
		out.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
		out.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;
		out.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;
		out.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
		out.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
		out.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

		return out;
	}

	Matrix &operator*=( const Matrix &other )
	{
		*this = *this * other;
		return *this;
	}

	// Members
	union
	{
		struct
		{
			f32 m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};
		f32 m[4][4];
	};

};

// Stream operators
ostream &operator<<( ostream &os, const Matrix &matrix );

// Matrix functions
Matrix MatIdentity();
Matrix MatTranspose( const Matrix &mat );
Matrix MatTranslate( Vector3f translation );
Matrix MatRotate( const Quaternion &q );
Matrix MatScale( Vector3f scale );
f32 MatGetDeterminant( const Matrix &mat );
Matrix MatInverse( const Matrix &mat );

ceEND
