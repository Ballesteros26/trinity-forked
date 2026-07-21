// Copyright (c) 2026 CCP Games

#pragma once

#include "Vector3.h"

struct Vector4;
struct Matrix;

namespace CcpMath
{
struct Sphere;
struct AxisAlignedEllipsoid;
struct Ray;

struct AxisAlignedBox
{
	constexpr AxisAlignedBox();
	constexpr AxisAlignedBox( const Vector3& min, const Vector3& max );
	explicit AxisAlignedBox( const Vector4& sphere );
	explicit AxisAlignedBox( const AxisAlignedEllipsoid& sphere );
	explicit AxisAlignedBox( const Sphere& sphere );

	bool IsInitialized() const;

	operator bool() const;
	bool operator==( const AxisAlignedBox& other ) const;
	bool operator!=( const AxisAlignedBox& other ) const;

	Vector3 Size() const;
	Vector3 Center() const;

	void IncludePoint( const Vector3& pos );
	void IncludeBox( const AxisAlignedBox& other );
	void IncludeSphere( const Sphere& sphere );

	void Include( const Vector3& pos );
	void Include( const AxisAlignedBox& other );
	void Include( const Sphere& sphere );

	bool IsPointInside( const Vector3& pos ) const;

	void Transform( const Matrix& transform );
	void Offset( const Vector3& by );
	void Grow( const Vector3& by );
	void Grow( float by );
	void Scale( float scale );

	bool Intersects( const AxisAlignedBox& other ) const;
	bool Intersects( const Ray& ray, Vector3& outIntersection ) const;

	template <typename T> 
	void EnumerateVertices( const T& cb ) const;

	Vector3 m_min;
	Vector3 m_max;
};

AxisAlignedBox Transform( const AxisAlignedBox& aabb, const Matrix& transform );
AxisAlignedBox Intersection( const AxisAlignedBox& a, const AxisAlignedBox& b );

}

#include "AxisAlignedBox_inline.h"