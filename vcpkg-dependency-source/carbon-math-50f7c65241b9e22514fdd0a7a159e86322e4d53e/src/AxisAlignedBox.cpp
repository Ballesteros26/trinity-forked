// Copyright (c) 2026 CCP Games

#include "AxisAlignedBox.h"
#include "Matrix.h"

namespace CcpMath
{

void AxisAlignedBox::Transform( const Matrix& transform )
{
	if( !IsInitialized() )
	{
		return;
	}
	Vector3 min( FLT_MAX, FLT_MAX, FLT_MAX );
	Vector3 max( -FLT_MAX, -FLT_MAX, -FLT_MAX );

	EnumerateVertices( [&]( const Vector3& vtx ) {
		auto pos = TransformCoord( vtx, transform );
		for( int component = 0; component < 3; ++component )
		{
			if( pos[component] < min[component] )
			{
				min[component] = pos[component];
			}
			if( pos[component] > max[component] )
			{
				max[component] = pos[component];
			}
		}
	} );
	m_max = max;
	m_min = min;
}

bool AxisAlignedBox::Intersects( const Ray& ray, Vector3& outIntersection ) const
{
	if( !IsInitialized() )
	{
		return false;
	}
	if( m_min == m_max )
	{
		// we are actually dealing with a point but not a box
		outIntersection = m_min;
		return LengthSq( Normalize( m_min - ray.origin ) - ray.direction ) < 1e10;
	}

	auto rd = 1.f / Length( ray.direction );
	auto t0 = ( m_min - ray.origin ) * rd;
	auto t1 = ( m_max - ray.origin ) * rd;

	auto s = Vector3( std::min( t0.x, t1.x ), std::min( t0.y, t1.y ), std::min( t0.z, t1.z ) );
	auto b = Vector3( std::max( t0.x, t1.x ), std::max( t0.y, t1.y ), std::max( t0.z, t1.z ) );

	float minT = std::max( s.x, std::max( s.y, s.z ) );
	float maxT = std::min( b.x, std::min( b.y, b.z ) );

	outIntersection = ray.origin + minT * ray.direction;
	return minT < maxT;
}


}