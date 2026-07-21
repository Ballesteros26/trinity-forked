// Copyright (c) 2026 CCP Games

#include "Sphere.h"
#include "Ray.h"
#include "AxisAlignedBox.h"
#include "Vector4.h"


namespace CcpMath
{
	
inline constexpr AxisAlignedEllipsoid::AxisAlignedEllipsoid() :
	center( 0, 0, 0 ),
	radii( 0, 0, 0 )
{
}

inline constexpr AxisAlignedEllipsoid::AxisAlignedEllipsoid( const Vector3& c, const Vector3& r ) :
	center( c ),
	radii( r )
{
}

/*
	Defines an axis aligned ellipsoid from an axis aligned box.
	For inner = true, the box is defining the maximum box that can be fitted inside of the ellipsoid
	For inner = false, the box is defining the minimum box that fits the ellipsoid
*/
inline AxisAlignedEllipsoid::AxisAlignedEllipsoid( const AxisAlignedBox& box, bool inner )
{
	center = box.Center();
	radii = 0.5f * box.Size();
	if( inner )
	{
		radii *= CcpMath::Constants::SQRT_THREE;	
	}
}

inline bool AxisAlignedEllipsoid::IsInitialized() const
{
	return *this != AxisAlignedEllipsoid();
}

inline AxisAlignedEllipsoid::operator bool() const
{
	return IsInitialized();
}

inline bool AxisAlignedEllipsoid::operator==( const AxisAlignedEllipsoid& other ) const
{
	return center == other.center && radii == other.radii;
}

inline bool AxisAlignedEllipsoid::operator!=( const AxisAlignedEllipsoid& other ) const
{
	return center != other.center || radii != other.radii;
}

inline void AxisAlignedEllipsoid::IncludePoint( const Vector3& point )
{
	if( IsPointInside( point ) )
	{
		return;
	}

	CcpMath::AxisAlignedBox box( *this );
	// scale it down so it is the biggest box that can be fitted within the ellipsoid
	box.Scale( CcpMath::Constants::ONE_OVER_SQRT_THREE );
	box.IncludePoint( point );

	// Scale up the box dimensions with sqrt(3) (because the ratio of box volume to ellipsoid volume is sqrt(3))
	// so we get the smallest ellipsoid to fit the box which now includes the point
	radii = box.Size() * 0.5f * CcpMath::Constants::SQRT_THREE;
	center = box.Center();
}

inline void AxisAlignedEllipsoid::IncludeBox( const AxisAlignedBox& box )
{
	if( IsPointInside( box.m_max ) && IsPointInside( box.m_min ) )
	{
		return;
	}

	CcpMath::AxisAlignedBox containingBox( *this );
	// scale it down so it is the biggest box that can be fitted within the ellipsoid
	containingBox.Scale( CcpMath::Constants::ONE_OVER_SQRT_THREE );
	containingBox.IncludeBox( box );

	// Scale up the box dimensions with sqrt(3) (because the ratio of box volume to ellipsoid volume is sqrt(3))
	// so we get the smallest ellipsoid to fit the box which now includes the point
	radii = containingBox.Size() * 0.5f * CcpMath::Constants::SQRT_THREE;
	center = containingBox.Center();
}

inline void AxisAlignedEllipsoid::IncludeSphere( const Sphere& sphere )
{
	// include a box that encapsulates the sphere
	IncludeBox( AxisAlignedBox( sphere ) );
}

inline bool AxisAlignedEllipsoid::IsPointInside( const Vector3& point ) const
{
	Vector3 d = point - center;
	d.x /= radii.x;
	d.y /= radii.y;
	d.z /= radii.z;
	return LengthSq( d ) <= 1;
}

inline void AxisAlignedEllipsoid::Offset( const Vector3& by )
{
	center += by;
}

inline void AxisAlignedEllipsoid::Scale( float scale )
{
	if( !IsInitialized() )
	{
		return;
	}
	radii *= scale;
}

inline void AxisAlignedEllipsoid::Scale( const Vector3& scale )
{
	if( !IsInitialized() )
	{
		return;
	}
	radii.x *= scale.x;
	radii.y *= scale.y;
	radii.z *= scale.z;
}

}
