// Copyright (c) 2026 CCP Games

#pragma once

#include "Requirements.h"

struct Vector2
{
	constexpr Vector2();
	constexpr Vector2( float x_, float y_ );
	Vector2( const XMVECTOR& other );

	operator XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector2& operator+=( const Vector2& other );
	Vector2& operator-=( const Vector2& other );
	Vector2& operator*=( const Vector2& other );
	Vector2& operator/=( const Vector2& other );
	Vector2& operator*=( float f );
	Vector2& operator/=( float f );

	Vector2 operator+() const;
	Vector2 operator-() const;

	const Vector2 operator+( const Vector2& other ) const;
	const Vector2 operator-( const Vector2& other ) const;
	const Vector2 operator*( const Vector2& other ) const;
	const Vector2 operator/( const Vector2& other ) const;

	const Vector2 operator*( float f ) const;
	const Vector2 operator/( float f ) const;

	bool operator==( const Vector2& other ) const;
	bool operator!=( const Vector2& other ) const;

	float x, y;
};

const Vector2 operator*( float f, const Vector2& other );
float Length( const Vector2& v );
Vector2 Normalize( const Vector2& v );

#include "Vector2_inline.h"