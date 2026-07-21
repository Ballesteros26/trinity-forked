// Copyright (c) 2026 CCP Games

#pragma once

#include "Requirements.h"

struct Vector3;
struct Vector4;
struct Matrix;

struct Quaternion
{
	constexpr Quaternion();
	constexpr Quaternion( float x, float y, float z, float w );
	Quaternion( const XMVECTOR& other );
	Quaternion( const Vector4& other );

	operator XMVECTOR() const;

	Quaternion& operator+=( const Quaternion& other );
	Quaternion& operator-=( const Quaternion& other );
	Quaternion& operator*=( const Quaternion& other );
	Quaternion& operator*=( float f );
	Quaternion& operator/=( float f );

	Quaternion operator+() const;
	Quaternion operator-() const;

	const Quaternion operator+( const Quaternion& other ) const;
	const Quaternion operator-( const Quaternion& other ) const;
	const Quaternion operator*( const Quaternion& other ) const;

	const Quaternion operator*( float f ) const;
	const Quaternion operator/( float f ) const;

	bool operator==( const Quaternion& other ) const;
	bool operator!=( const Quaternion& other ) const;

	float x, y, z, w;
};


const Quaternion operator*( float f, const Quaternion& other );


Quaternion IdentityQuaternion();

float LengthSq( const Quaternion& q );
float Length( const Quaternion& q );
Quaternion Normalize( const Quaternion& q );
Quaternion Inverse( const Quaternion& q );
Quaternion Conjugate( const Quaternion& q );
Quaternion Exp( const Quaternion& q );
float Dot( const Quaternion& q1, const Quaternion& q2 );

Quaternion RotationQuaternion( const Matrix& m );
Quaternion RotationQuaternion( float yaw, float pitch, float roll );
Quaternion RotationQuaternion( const Vector3& axis, float angle );

Quaternion Slerp( const Quaternion& q1, const Quaternion& q2, float t );
std::pair<Vector3, float> GetAxisAngle( const Quaternion& q );

#include "Quaternion_inline.h"