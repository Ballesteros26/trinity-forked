// Copyright (c) 2026 CCP Games

#include "Requirements.h"
#include "Vector3.h"

// --------------------------------------------------------------------------------------
void ComputeBoundingSphere(
	const Vector3* firstPosition,
	uint32_t numVertices,
	uint32_t stride,
	Vector3& center,
	float& radius )
{
	Vector3 temp( 0.0f, 0.0f, 0.0f );
	radius = 0.0f;
	if( numVertices == 0 )
	{
		center = temp;
		return;
	}

	const uint8_t* data = reinterpret_cast<const uint8_t*>( firstPosition );

	for( uint32_t i = 0; i < numVertices; i++ )
	{
		temp += *reinterpret_cast<const Vector3*>( data );
		data += stride;
	}

	center = temp / float( numVertices );
	data = reinterpret_cast<const uint8_t*>( firstPosition );

	for( uint32_t i = 0; i < numVertices; i++ )
	{
		float d = LengthSq( center - *reinterpret_cast<const Vector3*>( data ) );
		data += stride;
		if( d > radius )
		{
			radius = d;
		}
	}
	radius = sqrt( radius );
}
