// Copyright © 2011 CCP ehf.

#pragma once

#ifndef ITr2BoundingBox_h
#define ITr2BoundingBox_h

struct Vector3;
struct Vector4;

BLUE_INTERFACE( ITr2BoundingBox ) :
	IRoot
{
	virtual bool GetWorldBoundingBox( Vector3 & min, Vector3 & max ) const = 0;
	virtual bool IsBoundingBoxReady( void ) const = 0;

	// Optional tighter bound for genuinely spherical objects, as (center.xyz, radius.w).
	// Consumers that can exploit it (bracket projection) prefer it over the box.
	virtual bool GetWorldBoundingSphere( Vector4& /*sphere*/ ) const
	{
		return false;
	}
};

#endif