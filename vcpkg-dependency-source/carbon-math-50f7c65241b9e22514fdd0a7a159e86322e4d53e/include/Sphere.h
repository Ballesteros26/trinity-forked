// Copyright (c) 2026 CCP Games

#pragma once

#include "Vector3.h"

struct Vector4;
struct Matrix;

namespace CcpMath
{

struct AxisAlignedBox;

struct Sphere
{
	constexpr Sphere();
	constexpr Sphere( const Vector3& center, float radius );
	explicit Sphere( const Vector4& packedSphere );
	explicit Sphere( const AxisAlignedBox& box );
	Sphere( const AxisAlignedBox& box, const Matrix& transform );

	bool IsInitialized() const;

	operator bool() const;
	bool operator==( const Sphere& other ) const;
	bool operator!=( const Sphere& other ) const;

	void IncludePoint( const Vector3& pos );
	void IncludeSphere( const Sphere& sphere );

	void Include( const Vector3& pos );
	void Include( const Sphere& sphere );

	bool IsPointInside( const Vector3& pos ) const;
	bool IsSphereInside( const Sphere& sphere ) const;

	void Transform( const Matrix& transform );

	Vector3 center;
	float radius;
};

}

#include "Sphere_inline.h"

