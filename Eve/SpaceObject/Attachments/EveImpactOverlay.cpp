////////////////////////////////////////////////////////////
//
//    Created:   September 2015
//    Copyright: CCP 2015
//

#include "StdAfx.h"
#include "EveImpactOverlay.h"

#include "include/TriMath.h"
#include "Utilities/BoundingSphere.h"
#include "Tr2MeshBase.h"
#include "Eve/EveUpdateContext.h"

EveImpactOverlay::EveImpactOverlay( IRoot* lockobj ):
	m_display( true ),
	m_speed( 1.f ),
	m_maxShieldImpacts( 128 ),
	m_shieldEllipsoidCenter( 0.f, 0.f, 0.f ),
	m_shieldEllipsoidRadii( 1.f, 1.f, 1.f ),
	m_shieldImpactDataNextIdx( 1 ),
	m_armorImpactDataNextIdx( 1 ),
	m_shieldDataVar( "ImpactShieldDataMap", &m_shieldDataTexture )
{

	PrepareResources();
}

EveImpactOverlay::~EveImpactOverlay()
{
}

// --------------------------------------------------------------------------------
// Description:
//   If loading from a .red file, we now can start creating resources
// --------------------------------------------------------------------------------
bool EveImpactOverlay::Initialize()
{
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Release all device resources here
// --------------------------------------------------------------------------------
void EveImpactOverlay::ReleaseResources( TriStorage s )
{
	// get rid of data texture
	m_shieldDataTexture.Destroy();
}

// --------------------------------------------------------------------------------
// Description:
//   Debug/dev helper strings to show up in some tools
// --------------------------------------------------------------------------------
#ifdef TRINITYDEV
void EveImpactOverlay::GetDescription( std::string& desc )
{
	desc = std::string( "EveImpactOverlay" );
}
#endif

// --------------------------------------------------------------------------------
// Description:
//   Allocate all device resources here
// --------------------------------------------------------------------------------
bool EveImpactOverlay::OnPrepareResources()
{
	USE_MAIN_THREAD_RENDER_CONTEXT();

	// create the shield impact data texture here, prefill it with zeros
	std::vector<Vector4> prefillData( m_maxShieldImpacts * 2, Vector4( 0.f, 0.f, 0.f, 0.f ) );
	Tr2SubresourceData init = { &prefillData, m_maxShieldImpacts * uint32_t(sizeof(Vector4)), 2 * m_maxShieldImpacts * uint32_t(sizeof(Vector4)) };
	if( FAILED( m_shieldDataTexture.Create2D( m_maxShieldImpacts, 2, 1, Tr2RenderContextEnum::PIXEL_FORMAT_R32G32B32A32_FLOAT, Tr2RenderContextEnum::USAGE_CPU_WRITE, &init, renderContext ) ) )
	{
		return false;
	}

	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Trinity's way of providing batches to render
// --------------------------------------------------------------------------------
void EveImpactOverlay::UpdateSyncronous( EveUpdateContext& updateContext, EveSpaceObject2* parent )
{
	USE_MAIN_THREAD_RENDER_CONTEXT();

	// update data texture
	void* data = nullptr;
	uint32_t pitch = 0;
	if( SUCCEEDED( m_shieldDataTexture.Lock( 0, data, pitch, Tr2RenderContextEnum::LOCK_WRITEONLY, renderContext ) ) )
	{
		uint8_t* mem = (uint8_t*)data;

		// shield info in first lines
		uint32_t y = 0;
		for( uint32_t x = 0; x < m_shieldDataTexture.GetWidth(); ++x )
		{
			Vector4* texel = (Vector4*)&mem[pitch * y + 16 * x];

			// first one is special
			if( x == 0 )
			{
				texel->x = float( m_shieldTexelData.size() );
				texel->y = texel->z = texel->w = 0.f;
			}
			else
			{
				if( x - 1 < m_shieldTexelData.size() )
				{
					*texel = m_shieldTexelData[ x - 1 ].row0;
				}
				else
				{
					texel->x = texel->y = texel->z = texel->w = 0.f;
				}
			}
		}

		// armor data on second line
		y = 1;
		for( uint32_t x = 0; x < m_shieldDataTexture.GetWidth(); ++x )
		{
			Vector4* texel = (Vector4*)&mem[pitch * y + 16 * x];

			// first one is special
			if( x == 0 )
			{
				texel->x = float( m_armorTexelData.size() );
				texel->y = texel->z = texel->w = 0.f;
			}
			else
			{
				if( x - 1 < m_armorTexelData.size() )
				{
					*texel = m_armorTexelData[ x - 1 ].row0;
				}
				else
				{
					texel->x = texel->y = texel->z = texel->w = 0.f;
				}
			}
		}

		m_shieldDataTexture.Unlock( renderContext );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Trinity's way of providing batches to render
// --------------------------------------------------------------------------------
void EveImpactOverlay::UpdateAsyncronous( EveUpdateContext& updateContext, EveSpaceObject2* parent )
{
	// do all the math-heavy conversion here async

	// get parent's bounding ellipsoid and put it in word space
	Vector3 parentBBoxMin( -1.f, -1.f, -1.f ), parentBBoxMax( 1.f, 1.f, 1.f );
	if( parent->GetLocalBoundingBox( parentBBoxMin, parentBBoxMax ) )
	{
		m_shieldEllipsoidRadii = 0.5f * TRI_SQRT3 * ( parentBBoxMax - parentBBoxMin );
		m_shieldEllipsoidCenter = parentBBoxMin + 0.5f * ( parentBBoxMax - parentBBoxMin );

		// need the inverse world matrix
		Matrix parentWorldTransform, parentInverseWorldTransform;
		parent->GetLocalToWorldTransform( parentWorldTransform );
		if( !D3DXMatrixInverse( &parentInverseWorldTransform, nullptr, &parentWorldTransform ) )
		{
			parentInverseWorldTransform = parentWorldTransform;
		}

		m_shieldTexelData.resize( m_shieldImpactData.size() );
		size_t i = 0;
		for( auto sidit = m_shieldImpactData.begin(); sidit != m_shieldImpactData.end(); ++sidit )
		{
			ShieldImpactData* shieldData = &sidit->second;
			ShieldTexelData* texelData = &m_shieldTexelData[i];

			// convert position and direction into object space
			Vector3 tgtPosOS, dirOS;
			D3DXVec3TransformCoord( &tgtPosOS, &shieldData->targetPosition, &parentInverseWorldTransform );
			D3DXVec3TransformNormal( &dirOS, &shieldData->direction, &parentInverseWorldTransform );
			// intersections
			Vector3 p( 0.f, 0.f, 0.f );
			IntersectEllipsoidRay( p, m_shieldEllipsoidCenter, m_shieldEllipsoidRadii, tgtPosOS, dirOS );
			// "encode" it in texels
			texelData->row0 = Vector4( p, shieldData->timeLeft );
			// also need this intercept position in WS
			D3DXVec3TransformCoord( &shieldData->interceptPosition, &p, &parentWorldTransform );

			++i;
		}
	}

	// armor
	m_armorTexelData.resize( m_armorImpactData.size() );
	size_t i = 0;
	for( auto aidit = m_armorImpactData.begin(); aidit != m_armorImpactData.end(); ++aidit )
	{
		ArmorImpactData* armorData = &aidit->second;
		ArmorTexelData* texelData = &m_armorTexelData[i];

		// convert position
		texelData->row0 = Vector4( armorData->impactPosition, 0.f );

		++i;
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Trinity's way of providing batches to render
// --------------------------------------------------------------------------------
void EveImpactOverlay::GetBatches( ITriRenderBatchAccumulator* accumulator, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
	if( !m_display )
	{
		return;
	}
	if( !m_mesh )
	{
		return;
	}
	if( !m_shieldDataTexture.IsValid() )
	{
		return;
	}
	if( m_shieldTexelData.empty() )
	{
		return;
	}

	const Tr2MeshAreaVector* areas = m_mesh->GetAreas( batchType );
	m_mesh->GetBatches( accumulator, areas, perObjectData );
}

// --------------------------------------------------------------------------------
// Description:
//   Use this method to add a new shield impact
// --------------------------------------------------------------------------------
int EveImpactOverlay::CreateShieldImpact( const Vector3& position, const Vector3& direction )
{
	// fill our struct, but keep it in world space
	ShieldImpactData sid;
	sid.direction = direction;
	D3DXVec3Normalize( &sid.direction, &sid.direction );
	sid.targetPosition = position;
	sid.interceptPosition = Vector3( 0.f, 0.f, 0.f );
	sid.timeLeft = 0.f;
	m_shieldImpactData[ m_shieldImpactDataNextIdx ] = sid;
	return m_shieldImpactDataNextIdx++;
}

// --------------------------------------------------------------------------------
// Description:
//   Hand out the intercept position of a given impact
// --------------------------------------------------------------------------------
bool EveImpactOverlay::GetShieldImpactPosition( Vector3& out, int shieldImpactIndex ) const
{
	auto finder = m_shieldImpactData.find( shieldImpactIndex );
	if( finder == m_shieldImpactData.end() )
	{
		return false;
	}
	out = finder->second.interceptPosition;
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Use this method to add a new armor impact
// --------------------------------------------------------------------------------
int EveImpactOverlay::CreateArmorImpact( const Vector3& position )
{
	// fill our struct, but keep it in world space
	ArmorImpactData aid;
	aid.impactPosition = position;
	aid.timeLeft = 0.f;
	m_armorImpactData[ m_armorImpactDataNextIdx ] = aid;
	return m_armorImpactDataNextIdx++;
}

// --------------------------------------------------------------------------------
// Description:
//   Hand out the intercept position of a given impact
// --------------------------------------------------------------------------------
bool EveImpactOverlay::GetArmorImpactPosition( Vector3& out, int armorImpactIndex ) const
{
	auto finder = m_armorImpactData.find( armorImpactIndex );
	if( finder == m_armorImpactData.end() )
	{
		return false;
	}
	out = finder->second.impactPosition;
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Hand out the shader for armor efects
// --------------------------------------------------------------------------------
Tr2EffectPtr EveImpactOverlay::GetArmorDamageShader( TriBatchType batchType ) const
{
	if( batchType == TRIBATCHTYPE_DECAL )
	{
		return m_armorDamageShader;
	}
	return nullptr;
}




