// Copyright (c) 2026 CCP Games

#include "Sphere.h"
#include "AxisAlignedBox.h"
#include "Matrix.h"

namespace CcpMath
{

Sphere::Sphere( const AxisAlignedBox& box, const Matrix& transform )
{
	Vector3 min = TransformCoord( box.m_min, transform );
	Vector3 max = TransformCoord( box.m_max, transform );

	center = ( min + max ) * 0.5f;
	radius = Length( max - min ) * 0.5f;
}

void Sphere::Transform( const Matrix& transform )
{
	if( !IsInitialized() )
	{
		return;
	}
	// translate center
	center = TransformCoord( center, transform );
	// scale with highest scale factor
	float scaleX = LengthSq( transform.GetX() );
	float scaleY = LengthSq( transform.GetY() );
	float scaleZ = LengthSq( transform.GetZ() );
	float scale = sqrt( std::max( scaleX, std::max( scaleY, scaleZ ) ) );
	radius *= scale;
}
}