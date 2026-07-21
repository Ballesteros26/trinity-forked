// Copyright (c) 2026 CCP Games

#pragma once


struct Vector2;
struct Vector3;
struct Vector4;


struct Float_16
{
	constexpr Float_16();
	explicit Float_16( float other );
	constexpr explicit Float_16( uint16_t other );

	operator float() const;

	bool operator == ( const Float_16& ) const;
	bool operator != ( const Float_16& ) const;
protected:
	static uint16_t Float32To16( float in );
	static float Float16To32( const uint16_t in );

	uint16_t m_value;
};


struct Vector2_16
{
	constexpr Vector2_16();
	constexpr Vector2_16( Float_16 x_, Float_16 y_ );
	Vector2_16( float x_, float y_ );
	explicit Vector2_16( const Vector2& other );

	operator Vector2() const;

	Float_16 x, y;
};


struct Vector3_16
{
	constexpr Vector3_16();
	constexpr Vector3_16( Float_16 x_, Float_16 y_, Float_16 z_ );
	Vector3_16( float x_, float y_, float z_ );
	explicit Vector3_16( const Vector3& other );

	operator Vector3() const;

	Float_16 x, y, z;
};


struct Vector4_16
{
	constexpr Vector4_16();
	constexpr Vector4_16( Float_16 x_, Float_16 y_, Float_16 z_, Float_16 w_ );
	Vector4_16( float x_, float y_, float z_, float w_ );
	explicit Vector4_16( const Vector4& other );

	operator Vector4() const;

	Float_16 x, y, z, w;
};

#include "Float16_inline.h"