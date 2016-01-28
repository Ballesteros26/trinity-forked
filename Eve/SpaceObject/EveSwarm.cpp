////////////////////////////////////////////////////////////
//
//    Created:   2015
//    Copyright: CCP 2015
//
#include "StdAfx.h"
#include "EveSwarm.h"
#include "Eve/EveUpdateContext.h"
#include "Tr2MeshArea.h"
#include "Tr2MeshBase.h"

#include "include/TriMath.h"
#include "Utilities/BoundingBox.h"

#include "Eve/SpaceObject/Attachments/Sets/EveSpriteSet.h"
#include "Eve/SpaceObject/Attachments/Sets/EveSpotlightSet.h"

EveSwarmRenderable::EveSwarmRenderable( IRoot* lockobj )
{
	memset( &m_psData, 0, sizeof( EveSpaceObjectPSData ) );
	memset( &m_vsData, 0, sizeof( EveSpaceObjectVSData ) );
	
}
EveSwarmRenderable::~EveSwarmRenderable()
{
}

void EveSwarmRenderable::GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
	if( m_mesh )
	{
		m_mesh->GetBatches( batches, m_mesh->GetAreas( batchType ), perObjectData );
	}
}

void EveSwarmRenderable::GetShadowBatches( ITriRenderBatchAccumulator* batches, const Tr2PerObjectData* perObjectData )
{
	if( m_mesh )
	{
		m_mesh->GetBatches( batches, m_mesh->GetAreas( TRIBATCHTYPE_OPAQUE ), perObjectData );
	}
}

float EveSwarmRenderable::GetSortValue()
{
	Vector3 d = Tr2Renderer::GetViewPosition() - m_worldTransform.GetTranslation();
	float distance = D3DXVec3Length( &d );
	return distance;
}

Tr2PerObjectData* EveSwarmRenderable::GetPerObjectData( ITriRenderBatchAccumulator* accumulator )
{
	Tr2PerObjectDataWithPersistentBuffers<EveSwarmRenderable>* perObjectData = accumulator->Allocate<Tr2PerObjectDataWithPersistentBuffers<EveSwarmRenderable>>();
	if( !perObjectData )
	{
		return NULL;
	}
	perObjectData->Initialize( this, &m_perObjectDataVs, &m_perObjectDataPs );

	return perObjectData;
}

uint32_t EveSwarmRenderable::GetPerObjectDataSize( Tr2RenderContextEnum::ShaderType shaderType ) const
{
	if( shaderType == Tr2RenderContextEnum::PIXEL_SHADER )
	{
		return sizeof( m_psData );
	}
	else
	{
		return sizeof( m_vsData );
	}
}

void EveSwarmRenderable::UpdatePerObjectBuffer( Tr2RenderContextEnum::ShaderType shaderType, uint32_t size, void* data )
{
	if( shaderType == Tr2RenderContextEnum::PIXEL_SHADER )
	{
		uint8_t* perObjectPS = (uint8_t*)data;
		memcpy( perObjectPS, &m_psData, sizeof( m_psData ) );
	}
	else
	{
		uint8_t* perObjectVS = (uint8_t*)data;
		memcpy( perObjectVS, &m_vsData, sizeof( m_vsData ) );
	}
}

bool EveSwarmRenderable::HasTransparentBatches()
{
	if( m_mesh )
	{
		return !(m_mesh->GetAreas( TRIBATCHTYPE_TRANSPARENT )->empty());
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
// EveSwarmRenderable
void EveSwarmRenderable::SetMesh( Tr2MeshBase* mesh )
{
	m_mesh = mesh;
}

void EveSwarmRenderable::SetWorldTransform( const Matrix& transform )
{
	m_worldTransform = transform;
	m_vsData.worldTransformLast = m_vsData.worldTransform;
	D3DXMatrixTranspose( &m_vsData.worldTransform, &m_worldTransform );
	
	m_perObjectDataVs.InvalidateBufferData();
	m_perObjectDataPs.InvalidateBufferData();
}













Vector3 SwarmBehavior::CalculateForces( int i0, std::vector<SwarmVehicle>& swarmers, Vector3 anchorPosition, float timeSeconds )
{
	Vector3 force( 0, 0, 0 );
	float one_over_count = 0;
	if( swarmers.size() > 1 )
	{
		one_over_count = 1.f / ( swarmers.size() - 1 );
	}
	force += m_weightWander * Calculate_Wander( swarmers[i0], m_wanderDistance, m_wanderRadius, m_wanderFluctuation, timeSeconds );
	force += m_weightAnchor * Calculate_Cohesion( swarmers[i0].position, anchorPosition );
	for( unsigned i = 0; i < swarmers.size(); i++ )
	{
		if( i0 == i )
		{
			continue;
		}
		force += m_weightCohesion * Calculate_Cohesion( swarmers[i0].position, swarmers[i].position ) * one_over_count;
		//separation += m_weightSeparation * Calculate_Cohesion( m_swarmers[i].position, m_swarmers[i0].position ) * one_over_count;
		force += m_weightSeparation * Calculate_Separation( swarmers[i0].position, swarmers[i].position );// don't care about averages, if it's too close it's too close * one_over_count;
		force += swarmers[i].velocity * one_over_count * m_weightAlign;
		
	}
	return force;
}

Vector3 SwarmBehavior::Calculate_Cohesion( Vector3 p0, Vector3 p1 )
{
	Vector3 d;
	D3DXVec3Subtract( &d, &p1, &p0 );
	return d;
}

Vector3 SwarmBehavior::Calculate_Separation( Vector3 p0, Vector3 p1 )
{
	Vector3 d;
	D3DXVec3Subtract( &d, &p0, &p1 );
	float length = D3DXVec3Length( &d );
	if( length == 0.f )
	{
		return Vector3( TriRand() - 0.5f, TriRand() - 0.5f, TriRand() - 0.5f );
	}
	return *D3DXVec3Normalize( &d, &d ) * 1000.f / length;
}

Vector3 SwarmBehavior::Calculate_Wander( SwarmVehicle& s, float wanderDistance, float radius, float fluctuation, float t )
{
	// Evolve the target point on the 'sphere' around a point wanderDistance in front of our swarmer
	Vector3 target = s.wanderTarget;
	Vector3 newOffset = Vector3( 2*TriRand() - 1.f, 2*TriRand() - 1.f, 2*TriRand() - 1.f );
	D3DXVec3Normalize( &newOffset, &newOffset );
	newOffset *= fluctuation * radius * t;
	target += newOffset;
	D3DXVec3Normalize( &target, &target );
	target *= radius;
	s.wanderTarget = target;

	// And calculate the final target force
	if( D3DXVec3LengthSq( &s.velocity ) != 0 )
	{
		D3DXVec3Normalize( &target, &s.velocity );
	}
	else
	{
		// start in the wander direction if no velocity
		D3DXVec3Normalize( &target, &s.wanderTarget );
	}
	target = target * wanderDistance + s.wanderTarget;
	return target;
}





















EveSwarm::EveSwarm( IRoot* lockobj ) :
	PARENTLOCK( m_renderables ),
	m_rootVelocity( 0, 0, 0 ),
	m_rootPosition( 0, 0, 0 ),
	m_mass( 1.f ),
	m_maxAcceleration( 100.f ),
	m_maxSpeed( 100.f ),
	m_maxOffset( 0.f ),
	m_timeMultiplier( 1.f ),
	m_debugSize( 24.f ),
	m_count( 10 )
{
}

EveSwarm::~EveSwarm()
{
}

void EveSwarm::UpdateSyncronous( EveUpdateContext& updateContext ) {}
void EveSwarm::UpdateAsyncronous( EveUpdateContext& context )
{
	BoundingBoxInitialize( m_squadBoundsMin, m_squadBoundsMax );
	float timeSeconds = context.GetDeltaT() * m_timeMultiplier;
	if( timeSeconds > m_maxTime )
	{
		timeSeconds = m_maxTime;
	}

	// Calculate acceleration
	for( unsigned i = 0; i < m_vehicles.size(); i++ )
	{
		Vector3 force = m_behavior.CalculateForces( i, m_vehicles, m_rootPosition, timeSeconds );
		Vector3 acc = force * 1.f / m_mass;
		m_vehicles[i].acceleration += acc * timeSeconds;
		if( D3DXVec3Length( &m_vehicles[i].acceleration ) > m_maxAcceleration )
		{
			D3DXVec3Normalize( &m_vehicles[i].acceleration, &m_vehicles[i].acceleration );
			m_vehicles[i].acceleration *= m_maxAcceleration;
		}
	}

	// Update velocities and positions
	for( unsigned i = 0; i < m_vehicles.size(); i++ )
	{
		m_vehicles[i].velocity = m_vehicles[i].velocity + m_vehicles[i].acceleration * timeSeconds;
		if( D3DXVec3Length( &m_vehicles[i].velocity ) > m_maxSpeed )
		{
			D3DXVec3Normalize( &m_vehicles[i].velocity, &m_vehicles[i].velocity );
			m_vehicles[i].velocity *= m_maxSpeed;
		}
		m_vehicles[i].position += m_vehicles[i].velocity * timeSeconds;
	}

	// Update world transforms
	auto rit = m_renderables.begin();
	Vector3 baseDir( 0, 0, 1 );
	for( unsigned i = 0; i < m_vehicles.size() && rit != m_renderables.end(); i++, rit++ )
	{
		Matrix world;
		Quaternion rotation;
		Vector3 dir, cross;
		D3DXVec3Normalize( &dir, &(m_vehicles[i].velocity ) );
		float dot = D3DXVec3Dot( &baseDir, &dir );
		D3DXVec3Cross( &cross, &baseDir, &dir );
		D3DXQuaternionRotationAxis( &rotation, &cross, acos( dot ) );
		D3DXMatrixAffineTransformation( &world, 1.f, nullptr, &rotation, &(m_vehicles[i].position) );
		(*rit)->SetWorldTransform( world );
		BoundingBoxUpdate( m_squadBoundsMin, m_squadBoundsMax, m_vehicles[i].position );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Registers space object attachments (sprite and spotlight sets) with quad 
//   renderer.
// Arguments:
//   quadRenderer - quad renderer
// --------------------------------------------------------------------------------
void EveSwarm::RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer )
{
	/*for( auto it = m_spriteSets.begin(); it != m_spriteSets.end(); ++it )
	{
		// TODO: needs to be per set and swarmer so the hash used won't be good enough
		(*it)->RegisterWithQuadRenderer( quadRenderer );
	}
	for( auto it = m_spotlightSets.begin(); it != m_spotlightSets.end(); ++it )
	{
		(*it)->RegisterWithQuadRenderer( quadRenderer );
	}*/
}

// --------------------------------------------------------------------------------
// Description:
//   Adds sprites from sprite sets and spotlight sets to quad renderer.
// Arguments:
//   quadRenderer - quad renderer
// --------------------------------------------------------------------------------
void EveSwarm::AddQuadsToQuadRenderer( Tr2QuadRenderer& quadRenderer )
{
	/*if( !m_isInFrustum || !m_display )
	{
		return;
	}

	for( auto rit = m_renderables.begin(); rit != m_renderables.end(); ++rit )
	{
		for( auto it = m_spriteSets.begin(); it != m_spriteSets.end(); ++it )
		{
			// Assume no animation at least for now... and perhaps forever
			(*it)->AddToQuadRenderer( quadRenderer, (*rit)->GetWorldTransform(), 1, nullptr, 0 );
		}
		for( auto it = m_spotlightSets.begin(); it != m_spotlightSets.end(); ++it )
		{
			(*it)->AddToQuadRenderer( quadRenderer, (*rit)->GetWorldTransform(), 1, 1, nullptr, 0 );
		}
	}*/
}

void EveSwarm::RenderDebugInfo( Tr2RenderContext& renderContext )
{
	for( unsigned i = 0; i < m_vehicles.size(); i++ )
	{
		Vector3 pos = m_vehicles[i].position;
		Tr2Renderer::DrawSphere( pos, m_debugSize, 4, 0xffff00ff );
		Tr2Renderer::DrawLine( pos, pos + m_vehicles[i].velocity, 0xffff00ff );
		Tr2Renderer::DrawLine( pos, pos + m_vehicles[i].acceleration, 0xff0000ff );
	}
}


void EveSwarm::GetRenderables( const TriFrustum& frustum, std::vector<ITr2Renderable*>& renderables, const Matrix& parentTransform )
{
	for( auto it = m_renderables.begin(); it != m_renderables.end(); it++ )
	{
		// TODO: cull the bastard
		renderables.push_back( *it );
	}
}

bool EveSwarm::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const 
{
	sphere = Vector4( 0, 0, 0, 10000000.f );
	return true;
}

// This version of the function should perform an update on the model / ball position
void EveSwarm::GetModelCenterWorldPosition( Vector3 &position, Be::Time t )
{
	position = ( m_squadBoundsMax + m_squadBoundsMin ) * 0.5f;
}

// This version of the function should not update the object
void EveSwarm::GetCurrentModelCenterWorldPosition( Vector3 &position )
{
	position = ( m_squadBoundsMax + m_squadBoundsMin ) * 0.5f;
}

// If possible, return an AABB in local coordinates
bool EveSwarm::GetLocalBoundingBox( Vector3 &min, Vector3 &max )
{
	min = m_squadBoundsMin;
	max = m_squadBoundsMax;
	return true;
}
// Get the local to world transform
void EveSwarm::GetLocalToWorldTransform( Matrix &transform ) const 
{
	D3DXMatrixIdentity( &transform );
}

bool EveSwarm::Initialize()
{
	for( int i = 0; i < m_count; i++ )
	{
		AddSwarmer();
	}
	return true;
}

bool EveSwarm::OnModified( Be::Var* val )
{
	for( auto it = m_renderables.begin(); it != m_renderables.end(); it++ )
	{
		(*it)->SetMesh( m_mesh );
	}
	return true;
}

void EveSwarm::AddSwarmer()
{
	EveSwarmRenderablePtr renderable;
	renderable.CreateInstance();
	renderable->SetMesh( m_mesh );
	m_renderables.Append( renderable->GetRootObject() );
	m_vehicles.push_back( SwarmVehicle() );
}