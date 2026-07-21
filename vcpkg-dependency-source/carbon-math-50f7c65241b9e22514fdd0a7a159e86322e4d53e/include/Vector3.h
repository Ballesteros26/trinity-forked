// Copyright (c) 2026 CCP Games

#pragma once

#include "Requirements.h"

struct Vector2;

struct Vector3
{
	constexpr Vector3();
	constexpr Vector3( float x, float y, float z );
	Vector3( const XMVECTOR& other );

	operator XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector3& operator+=( const Vector3& other );
	Vector3& operator-=( const Vector3& other );
	Vector3& operator*=( const Vector3& other );
	Vector3& operator*=( float f );
	Vector3& operator/=( float f );

	Vector3 operator+() const;
	Vector3 operator-() const;

	const Vector3 operator+( const Vector3& other ) const;
	const Vector3 operator-( const Vector3& other ) const;
	const Vector3 operator*( const Vector3& other ) const;
	const Vector3 operator*( float f ) const;
	const Vector3 operator/( float f ) const;

	bool operator==( const Vector3& other ) const;
	bool operator!=( const Vector3& other ) const;

	const Vector2& GetXY() const;
	Vector2& GetXY();

	float x, y, z;
};


const Vector3 operator*( float f, const Vector3& other );

float Dot( const Vector3& v1, const Vector3& v2 );
float LengthSq( const Vector3& v );
float Length( const Vector3& v );
float AngleFromNonNormalized( const Vector3& v1, const Vector3& v2 );
float AngleFromNormalized( const Vector3& v1, const Vector3& v2 );
Vector3 Normalize( const Vector3& v );
Vector3 Cross( const Vector3& v1, const Vector3& v2 );
Vector3 ClampLength( const Vector3& v, float maxLength );
Vector3 Lerp( const Vector3& v1, const Vector3& v2, float s );
Vector3 Maximize( const Vector3& v1, const Vector3& v2 );
Vector3 Minimize( const Vector3& v1, const Vector3& v2 );
Vector3 Abs( const Vector3& v );
Vector3 Hermite( const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float s );

bool SphereBoundProbe(
	const Vector3& center,
	float radius,
	const Vector3& rayPosition,
	const Vector3& rayDirection );

void ComputeBoundingSphere(
	const Vector3* firstPosition,
	uint32_t numVertices,
	uint32_t stride,
	Vector3& center,
	float& radius );

#include "Vector3_inline.h"