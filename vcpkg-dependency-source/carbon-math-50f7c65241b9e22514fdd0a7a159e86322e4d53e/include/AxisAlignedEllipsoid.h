// Copyright (c) 2026 CCP Games

#pragma once

#include "Vector3.h"
#include "NumConst.h"

struct Vector4;
struct Matrix;

namespace CcpMath
{
struct Sphere;
struct AxisAlignedBox;

struct AxisAlignedEllipsoid
{
	constexpr AxisAlignedEllipsoid();
	constexpr AxisAlignedEllipsoid( const Vector3& center, const Vector3& radius );
	AxisAlignedEllipsoid( const AxisAlignedBox& box, bool inner );

	bool IsInitialized() const;

	operator bool() const;
	bool operator==( const AxisAlignedEllipsoid& other ) const;
	bool operator!=( const AxisAlignedEllipsoid& other ) const;

	void IncludePoint( const Vector3& pos );
	void IncludeBox( const AxisAlignedBox& other );
	void IncludeSphere( const Sphere& sphere );

	bool IsPointInside( const Vector3& pos ) const;

	void Scale( float scale );
	void Scale( const Vector3& scale );
	void Offset( const Vector3& by );

	Vector3 radii;
	Vector3 center;
};

}

#include "AxisAlignedEllipsoid_inline.h"