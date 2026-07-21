// Copyright (c) 2026 CCP Games

#pragma once

struct Vector4;

struct Color
{
	constexpr Color();
	Color( uint32_t argb );
	Color( const Vector4& );
	constexpr Color( float r, float g, float b, float a );

	operator uint32_t() const;
	operator Vector4() const;

	Color& operator += ( const Color& );
	Color& operator -= ( const Color& );
	Color& operator *= ( float );
	Color& operator /= ( float );

	// unary operators
	Color operator + () const;
	Color operator - () const;

	// binary operators
	Color operator + ( const Color& ) const;
	Color operator - ( const Color& ) const;
	Color operator * ( float ) const;
	Color operator / ( float ) const;

	bool operator == ( const Color& ) const;
	bool operator != ( const Color& ) const;

	float r, g, b, a;
};

Color Lerp( const Color& v1, const Color& v2, float s );
Color Saturate( const Color& v, float s );

#include "Color_inline.h"