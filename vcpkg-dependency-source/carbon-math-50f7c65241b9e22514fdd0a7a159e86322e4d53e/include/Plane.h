// Copyright (c) 2026 CCP Games

#pragma once

#include <utility>

struct Vector3;

struct Plane
{
	constexpr Plane();
	constexpr Plane( float a, float b, float c, float d );

	// assignment operators
	Plane& operator *= ( float );
	Plane& operator /= ( float );

	// unary operators
	Plane operator + () const;
	Plane operator - () const;

	// binary operators
	Plane operator * ( float ) const;
	Plane operator / ( float ) const;

	bool operator == ( const Plane& ) const;
	bool operator != ( const Plane& ) const;

	float a, b, c, d;
};


Plane operator * ( float scale, const Plane& plane );

std::pair<bool, Vector3> IntersectLine( const Plane& p, const Vector3& v1, const Vector3& v2 );

Plane Normalize( const Plane& plane );
float DotCoord( const Plane& p, const Vector3& v );
float DotNormal( const Plane& p, const Vector3& v );

#include "Plane_inline.h"