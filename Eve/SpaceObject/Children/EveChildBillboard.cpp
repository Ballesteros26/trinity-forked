////////////////////////////////////////////////////////////
//
//    Created:   June 2015
//    Copyright: CCP 2015
//
#include "StdAfx.h"
#include "EveChildBillboard.h"

#include "Tr2MeshArea.h"
#include "Tr2MeshBase.h"
#include "TriFrustum.h"
#include "Tr2PointLight.h"

#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/EveTransform.h"
#include "Resources/TriGeometryRes.h"
#include "Utilities/BoundingSphere.h"

extern float g_eveSpaceSceneLODFactor;


EveChildBillboard::EveChildBillboard( IRoot* lockobj ):
	EveChildTransform(),
	PARENTLOCK( m_lights ),
	m_boundingSphere( 0.0, 0.0, 0.0, -1.0),
	m_minScreenSize( 0.f ),
	m_display( true ),
	m_isVisible( false )
{
}

EveChildBillboard::~EveChildBillboard()
{
}

bool EveChildBillboard::Initialize()
{
	if( m_staticTransform )
	{
		RebuildLocalTransform();
	}
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Setup function to set data from outside, in this case just pass it to
//   function of base class
// --------------------------------------------------------------------------------
void EveChildBillboard::Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible )
{
	EveChildTransform::Setup( scale, rotation, translation, lowestLodVisible );
}

void EveChildBillboard::UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform, Tr2Lod parentLod )
{
	Vector4 boundingSphere;
	m_isVisible = false;

	if( !m_display )
	{
		return;
	}

	if( GetBoundingSphere( boundingSphere ) && frustum.IsSphereVisible( &boundingSphere ) && frustum.GetPixelSizeAccross( &boundingSphere ) >= m_minScreenSize * g_eveSpaceSceneLODFactor )
	{
		m_isVisible = true;
	}
}

void EveChildBillboard::GetRenderables( std::vector<ITr2Renderable*>& renderables )
{
	if( m_isVisible )
	{
		renderables.push_back( this );
	}
}

bool EveChildBillboard::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const
{
	Vector3 minBounds, maxBounds;
	if( m_mesh && m_mesh->GetBoundingBox( minBounds, maxBounds ) )
	{
		BoundingSphereFromBox( sphere, minBounds, maxBounds, &m_worldTransform );
	}
	else
	{
		return false;
	}

	return true;
}

bool EveChildBillboard::HasTransparentBatches()
{
	if( m_display && m_mesh )
	{
		return !(m_mesh->GetAreas( TRIBATCHTYPE_TRANSPARENT )->empty());
	}

	return false;
}

void EveChildBillboard::GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
	if( m_display && m_mesh )
	{
		m_mesh->GetBatches( batches, m_mesh->GetAreas( batchType ), perObjectData );
	}
}

void EveChildBillboard::GetShadowBatches( ITriRenderBatchAccumulator* batches, const Tr2PerObjectData* perObjectData )
{
	if( m_display && m_mesh )
	{
		m_mesh->GetBatches( batches, m_mesh->GetAreas( TRIBATCHTYPE_OPAQUE ), perObjectData );
	}
}

float EveChildBillboard::GetSortValue()
{
	Vector3 d = Tr2Renderer::GetViewPosition() - m_worldTransform.GetTranslation();
	float distance = Length( d );
	return distance;
}


void EveChildBillboard::UpdateSyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent )
{
}

void EveChildBillboard::UpdateAsyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent )
{
	Matrix parentTransform;
	if( spaceObjectParent )
	{
		spaceObjectParent->GetLocalToWorldTransform( parentTransform );
	}
	else if ( childParent )
	{
		childParent->GetLocalToWorldTransform( parentTransform );
	}
	else
	{
		return;
	}

	UpdateTransform( parentTransform );
	
	// Do the billboard magic

	Matrix invView;
	const Vector3& pos = m_worldTransform.GetTranslation();
	const Vector3& camPos = Tr2Renderer::GetViewPosition();
	Vector3 up( 0.0f, 1.0f, 0.0f );
	invView = Transpose( LookAtMatrix( camPos, pos, up ) );

	float parentScaleX = Length( parentTransform.GetX() );
	float parentScaleY = Length( parentTransform.GetY() );
	float parentScaleZ = Length( parentTransform.GetZ() );
	Vector3 finalScale = m_scaling;
	finalScale.x *= parentScaleX;
	finalScale.y *= parentScaleY;
	finalScale.z *= parentScaleZ;

	m_worldTransform._11 = invView._11 * finalScale.x;
	m_worldTransform._12 = invView._12 * finalScale.x;
	m_worldTransform._13 = invView._13 * finalScale.x;
	m_worldTransform._21 = invView._21 * finalScale.y;
	m_worldTransform._22 = invView._22 * finalScale.y;
	m_worldTransform._23 = invView._23 * finalScale.y;
	m_worldTransform._31 = invView._31 * finalScale.z;
	m_worldTransform._32 = invView._32 * finalScale.z;
	m_worldTransform._33 = invView._33 * finalScale.z;
}

void EveChildBillboard::GetLocalToWorldTransform( Matrix& transform ) const
{
	transform = m_worldTransform;
}

Tr2PerObjectData* EveChildBillboard::GetPerObjectData( ITriRenderBatchAccumulator* accumulator )
{
	EveBasicPerObjectData* data = accumulator->Allocate<EveBasicPerObjectData>();

	if( !data )
	{
		return NULL;
	}

	// column_major for shaders
	data->m_world = Transpose( m_worldTransform );
	data->m_worldInverseTranspose = Inverse( m_worldTransform );

	return data;
}

void EveChildBillboard::GetLights( Tr2LightManager& lightManager ) const
{
	XMMATRIX worldTransform = m_worldTransform;
	float scaling = XMVectorGetX( XMVectorAdd( XMVector3LengthEst( m_worldTransform.GetX() ),
		XMVectorAdd( XMVector3LengthEst( m_worldTransform.GetY() ), XMVector3LengthEst( m_worldTransform.GetZ() ) ) ) ) / 3.f;
	for( auto it = std::begin( m_lights ); it != std::end( m_lights ); ++it )
	{
		( *it )->AddLight( lightManager, worldTransform, scaling );
	}
}