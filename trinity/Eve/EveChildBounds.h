// Copyright © 2026 Fenris Creations

#pragma once
#ifndef EveChildBounds_H
#define EveChildBounds_H

#include "ITr2BoundingBox.h"
#include "Utilities/BoundingBox.h"
#include "Utilities/BoundingSphere.h"

// Best-effort world-space bounds for a scene-graph child: prefers the child's
// axis-aligned box when it implements ITr2BoundingBox, otherwise falls back to
// its bounding sphere. ChildT only needs GetBoundingSphere( Vector4&,
// BoundingSphereQuery ); there is no common bounds interface across the Eve
// child types (IEveTransform, IEveSpaceObjectChild, ...).
template <typename ChildT>
bool GetChildWorldBounds( const ChildT* child, Vector3& min, Vector3& max )
{
	if( auto childBoundingBox = dynamic_cast<const ITr2BoundingBox*>( child ) )
	{
		if( childBoundingBox->GetWorldBoundingBox( min, max ) )
		{
			return true;
		}
	}

	Vector4 childSphere;
	if( child->GetBoundingSphere( childSphere, EVE_BOUNDS_WITH_CHILDREN ) && BoundingSphereIsValid( childSphere ) )
	{
		BoundingBoxInitialize( childSphere, min, max );
		return true;
	}

	return false;
}

#endif // EveChildBounds_H
