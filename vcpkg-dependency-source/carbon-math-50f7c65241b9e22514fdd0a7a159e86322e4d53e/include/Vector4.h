// Copyright (c) 2026 CCP Games

#pragma once

#include "Requirements.h"

struct Vector3;

struct Vector4
{
	constexpr Vector4();
	constexpr Vector4( float x, float y, float z, float w );
	Vector4( const Vector3& xyz, float w );
	explicit Vector4( const XMVECTOR& other );

	operator XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector4& operator+=( const Vector4& other );
	Vector4& operator-=( const Vector4& other );
	Vector4& operator*=( const Vector4& other );
	Vector4& operator*=( float f );
	Vector4& operator/=( float f );

	Vector4 operator+() const;
	Vector4 operator-() const;

	const Vector4 operator+( const Vector4& other ) const;
	const Vector4 operator-( const Vector4& other ) const;
	const Vector4 operator*( const Vector4& other ) const;
	const Vector4 operator*( float f ) const;
	const Vector4 operator/( float f ) const;

	bool operator==( const Vector4& other ) const;
	bool operator!=( const Vector4& other ) const;

	const Vector3& GetXYZ() const;
	Vector3& GetXYZ();

	float x, y, z, w;
};

const Vector4 operator*( float f, const Vector4& other );
float Dot( const Vector4& v1, const Vector4& v2 );
Vector4 Cross( const Vector4& v1, const Vector4& v2, const Vector4& v3 );

#include "Vector4_inline.h"