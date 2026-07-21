// Copyright (c) 2026 CCP Games

#include "Vector4.h"
#include "AxisAlignedBox.h"

namespace CcpMath
{

inline constexpr Sphere::Sphere() :
	center( 0, 0, 0 ),
	radius( -1 )
{
}

inline constexpr Sphere::Sphere( const Vector3& center_, float radius_ ) :
	center( center_ ),
	radius( radius_ )
{
}

inline Sphere::Sphere( const Vector4& packedSphere ) :
	center( packedSphere.GetXYZ() ),
	radius( packedSphere.w )
{
}

inline Sphere::Sphere( const AxisAlignedBox& box ) :
	center( ( box.m_min + box.m_max ) * 0.5f ),
	radius( Length( box.m_max - box.m_min ) * 0.5f )
{
}

inline bool Sphere::IsInitialized() const
{
	return radius >= 0;
}

inline Sphere::operator bool() const
{
	return IsInitialized();
}

inline bool Sphere::operator==( const Sphere& other ) const
{
	return center == other.center && radius == other.radius;
}

inline bool Sphere::operator!=( const Sphere& other ) const
{
	return center != other.center || radius != other.radius;
}

inline void Sphere::IncludePoint( const Vector3& pos )
{
	if( !IsInitialized() )
	{
		center = pos;
		radius = 0;
		return;
	}
	if( IsPointInside( pos ) )
	{
		return;
	}
	Vector3 delta = pos - center;
	float deltaLen = Length( delta );

	center += 0.5f * ( 1.f - radius / deltaLen ) * delta;
	radius = 0.5f * ( radius + deltaLen );
}

inline void Sphere::IncludeSphere( const Sphere& sphere )
{
	if( !sphere.IsInitialized() )
	{
		return;
	}
	if( !IsInitialized() )
	{
		*this = sphere;
		return;
	}
	// do not update if is inside
	if( IsSphereInside( sphere ) || sphere.IsSphereInside( *this ) )
	{
		return;
	}

	// extend sphere
	Vector3 delta = sphere.center - center;
	float deltaLen = Length( delta );

	center += 0.5f * ( 1.f + ( sphere.radius - radius ) / deltaLen ) * delta;
	radius = 0.5f * ( radius + sphere.radius + deltaLen );
}

inline void Sphere::Include( const Vector3& pos )
{
	IncludePoint( pos );
}

inline void Sphere::Include( const Sphere& sphere )
{
	IncludeSphere( sphere );
}

inline bool Sphere::IsPointInside( const Vector3& pos ) const
{
	if( !IsInitialized() )
	{
		return false;
	}
	const float radiusEpsilon = 1e-4f;

	Vector3 delta = pos - center;
	return LengthSq( delta ) <= radius * radius + radiusEpsilon;
}

inline bool Sphere::IsSphereInside( const Sphere& sphere ) const
{
	if( !IsInitialized() || !sphere.IsInitialized() )
	{
		return false;
	}
	// pre-chck radiuses
	if( radius < sphere.radius )
	{
		return false;
	}
	Vector3 delta = sphere.center - center;
	return ( LengthSq( delta ) <= ( radius - sphere.radius ) * ( radius - sphere.radius ) );
}
}