#include "StdAfx.h"
#include "EveEffectRoot2.h"

#include "Utilities/BoundingSphere.h"
#include "TriFrustum.h"
#include "Tr2PointLight.h"
#include "Tr2LightManager.h"
#include "Eve/EveUpdateContext.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/SpaceObject/Children/EveChildContainer.h"

extern float g_eveSpaceObjectResourceUnloadingTimeThreshold;
extern float g_eveSpaceSceneMediumDetailThreshold;
extern float g_eveSpaceSceneLowDetailThreshold;

EveEffectRoot2::EveEffectRoot2( IRoot* lockobj ) :
	PARENTLOCK( m_observers ),
	PARENTLOCK( m_lights ),
	PARENTLOCK( m_effectChildren ),
	m_boundingSphere( 0, 0, 0, 0 ),
	m_scaling( 1.0f, 1.0f, 1.0f ),
	m_rotation( 0.0f, 0.0f, 0.0f, 1.0f ),
	m_translation( 0.0f, 0.0f, 0.0f ),
	m_estimatedSize( 0.0f ),
	m_display( true ),
	m_startTime( 0 ),
	m_effectDuration( -1 ),
	m_lodLevel( TR2_LOD_HIGH ),
	m_dynamicLODSelection( false ),
	m_secondaryLightingSphereRadiusLocal( 0.5f ),
	m_secondaryLightingSphereRadiusWorld( 0.5f ),
	m_secondaryLightingEmissiveColor( 0.f, 0.f, 0.f, 0.f )
{
}

bool EveEffectRoot2::Initialize()
{
	return true;
}

void EveEffectRoot2::UpdateSyncronous( EveUpdateContext& updateContext ) 
{	
	CCP_STATS_ZONE( __FUNCTION__ );

	UpdateWorldTransform( updateContext.GetTime() );

	D3DXMatrixTransformation( &m_localTransform, 0, 0, &m_scaling, 0, &m_rotation, &m_translation );
	D3DXMatrixMultiply( &m_lastUpdateMatrix, &m_localTransform, &m_worldTransform );
	m_secondaryLightingSphereRadiusWorld = m_secondaryLightingSphereRadiusLocal * ( m_scaling.x + m_scaling.y + m_scaling.z ) / 3.f;

	for( TriObserverLocalVector::iterator it = m_observers.begin(); it != m_observers.end(); ++it )
	{
		(*it)->Update( m_lastUpdateMatrix );
	}

	for( auto ecIt = m_effectChildren.begin(); ecIt != m_effectChildren.end(); ++ecIt ) 
	{
		(*ecIt)->UpdateSyncronous( updateContext, this, nullptr );
	}
}

void EveEffectRoot2::UpdateAsyncronous( EveUpdateContext& updateContext ) 
{	
	for( auto ecIt = m_effectChildren.begin(); ecIt != m_effectChildren.end(); ++ecIt ) 
	{
		(*ecIt)->UpdateAsyncronous( updateContext, this, nullptr );
	}
}

void EveEffectRoot2::RenderDebugInfo( Tr2RenderContext& renderContext ) 
{

}


void EveEffectRoot2::GetRenderables( const TriFrustum& frustum, std::vector<ITr2Renderable*>& renderables, const Matrix& parentTransform ) 
{
	if( !m_display )
	{
		return;
	}

	if( m_dynamicLODSelection )
	{
		Vector4 boundingSphere;
		GetBoundingSphere( boundingSphere );
		BoundingSphereTransform( m_worldTransform, boundingSphere );
		
		Tr2Lod oldLod = m_lodLevel;
		m_lodLevel = TR2_LOD_LOW;
		if( frustum.IsSphereVisible( &boundingSphere ) )
		{
			m_estimatedSize = frustum.GetPixelSizeAccross( &boundingSphere );
			if( m_estimatedSize >= g_eveSpaceSceneMediumDetailThreshold )
			{
				m_lodLevel = TR2_LOD_HIGH;
			}
			else if( m_estimatedSize >= g_eveSpaceSceneLowDetailThreshold )
			{
				m_lodLevel = TR2_LOD_MEDIUM;
			}
		}

		if ( oldLod != m_lodLevel )
		{
			for( auto ecIt = m_effectChildren.begin(); ecIt != m_effectChildren.end(); ++ecIt ) 
			{
				(*ecIt)->ChangeLOD( m_lodLevel );
			}
		}
	}


	for( auto ecIt = m_effectChildren.begin(); ecIt != m_effectChildren.end(); ++ecIt )
	{
		(*ecIt)->GetRenderables( frustum, renderables, m_worldTransform, m_lodLevel );
	}
}


bool EveEffectRoot2::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const
{ 
	sphere = m_boundingSphere;
	return true;
};


void EveEffectRoot2::UpdateWorldTransform( Be::Time time )
{	
	Quaternion rotation;
	Vector3 translation;

	if( m_ballPosition )
	{
		m_ballPosition->Update( &translation, time );
	}
	else
	{
		translation = Vector3( 0.0f, 0.0f, 0.0f );
	}

	if( m_ballRotation )
	{
		m_ballRotation->Update( &rotation, time );
	}
	else
	{
		rotation = Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
	}

	D3DXMatrixTransformation( &m_worldTransform, NULL, NULL, NULL, NULL, &rotation, &translation );
}


void EveEffectRoot2::UpdateModelCenterWorldPosition( Vector3 &position, Be::Time t )
{
	// This version of the function should perform an update on the model / ball position
	Matrix currentTransform;
	UpdateWorldTransform( t );
	D3DXMatrixTransformation( &currentTransform, 0, 0, &m_scaling, 0, &m_rotation, &m_translation );
	D3DXMatrixMultiply( &currentTransform, &currentTransform, &m_worldTransform );

	D3DXVec3TransformCoord( &position, (Vector3*)&m_boundingSphere, &currentTransform );
}

void EveEffectRoot2::GetModelCenterWorldPosition( Vector3 &position ) const
{
	// This version of the function does not perform an update on the object
	D3DXVec3TransformCoord( &position, (Vector3*)&m_boundingSphere, &m_lastUpdateMatrix );
}


bool EveEffectRoot2::GetLocalBoundingBox( Vector3 &min, Vector3 &max )
{
	// If possible, return an AABB in local coordinates
	return false;
}

void EveEffectRoot2::GetLocalToWorldTransform( Matrix &transform ) const
{
	// Get the local to world transform
	transform = m_lastUpdateMatrix;
}

void EveEffectRoot2::GetLights( Tr2LightManager& lightManager ) const
{
	XMMATRIX worldTransform = m_lastUpdateMatrix;
	float scaling = XMVectorGetX( XMVectorAdd( XMVector3LengthEst( m_lastUpdateMatrix.GetX() ), 
		XMVectorAdd( XMVector3LengthEst( m_lastUpdateMatrix.GetY() ), XMVector3LengthEst( m_lastUpdateMatrix.GetZ() ) ) ) ) / 3.f;
	for( auto it = std::begin( m_lights ); it != std::end( m_lights ); ++it )
	{
		( *it )->AddLight( lightManager, worldTransform, scaling );
	}
	for( auto it = m_effectChildren.begin(); it != m_effectChildren.end(); ++it )
	{
		( *it )->GetLights( lightManager );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Called by all children. It is similar to what spaceobjects send to vs/ps
// --------------------------------------------------------------------------------
void EveEffectRoot2::GetPerObjectStructs( EveSpaceObjectVSData& vsData, EveSpaceObjectPSData& psData ) const
{
	// vs
	memset( &vsData, 0, sizeof( EveSpaceObjectVSData ) );
	// activation
	vsData.shipData.y = 1.f;
	// boundingsphere
	vsData.shipData.w = 1.f;

	// ps
	memset( &psData, 0, sizeof( EveSpaceObjectPSData ) );
	// activation
	psData.shipData.y = 1.f;
	// boundingsphere
	psData.shipData.w = 1.f;
}

void EveEffectRoot2::RegisterSecondaryLightSource( Tr2ShLightingManager& manager )
{
	static const Color s_noAlbedoColor( 0.f, 0.f, 0.f, 0.f );
	manager.RegisterSecondaryLightSource( 
		&m_worldTransform.GetTranslation(), 
		&m_secondaryLightingSphereRadiusWorld, 
		&s_noAlbedoColor, 
		&m_secondaryLightingEmissiveColor );
}

void EveEffectRoot2::UnregisterSecondaryLightSource( Tr2ShLightingManager& manager )
{
	manager.UnregisterSecondaryLightSource( &m_worldTransform.GetTranslation() );
}

// --------------------------------------------------------------------------------
// Description:
//   Plays all "top level" curve sets.
// --------------------------------------------------------------------------------
void EveEffectRoot2::Start()
{
	for( auto cit = m_effectChildren.begin(); cit != m_effectChildren.end(); cit++ )
	{
		EveChildContainerPtr cont;
		if( (*cit)->QueryInterface(BlueInterfaceIID<EveChildContainer>(), (void**)&cont, BEQI_SILENT ) )
		{
			cont->PlayAllCurveSets();
		}
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Stops all "top level" curve sets.
// --------------------------------------------------------------------------------
void EveEffectRoot2::Stop()
{
	for( auto cit = m_effectChildren.begin(); cit != m_effectChildren.end(); cit++ )
	{
		EveChildContainerPtr cont;
		if( (*cit)->QueryInterface(BlueInterfaceIID<EveChildContainer>(), (void**)&cont, BEQI_SILENT ) )
		{
			cont->StopAllCurveSets();
		}
	}
}