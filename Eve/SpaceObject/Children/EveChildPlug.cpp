////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//
#include "StdAfx.h"
#include "EveChildPlug.h"

#include "Utilities/BoundingSphere.h"
#include "Eve/EveUpdateContext.h"
#include "Tr2ExternalParameter.h"


EveChildPlug::EveChildPlug( IRoot* lockobj ) :
	EveChildTransform(),
	PARENTLOCK( m_objects ),
	PARENTLOCK( m_externalParameters ),
	m_display( true )
{
}

EveChildPlug::~EveChildPlug()
{
}

bool EveChildPlug::Initialize()
{
	return true;
}

const char* EveChildPlug::GetName() const
{
	return m_name.c_str();
}

void EveChildPlug::SetName( const char* name )
{
	m_name = BlueSharedString( name );
}

void EveChildPlug::UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform, Tr2Lod parentLod )
{
	if( !m_display )
	{
		return;
	}
	
	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->UpdateVisibility( frustum, parentTransform, parentLod );
	}
}

void EveChildPlug::GetRenderables( std::vector<ITr2Renderable*>& renderables )
{
	if( !m_display )
	{
		return;
	}

	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->GetRenderables( renderables );
	}
}

bool EveChildPlug::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const
{
	bool success = false;
	Vector4 bSphere( 0.f, 0.f, 0.f, -1.f );
	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		if( (*it)->GetBoundingSphere( bSphere ) )
		{
			BoundingSphereSetOrUpdate( bSphere, sphere, success );
			success = true;
		}
	}
	return success;
}

void EveChildPlug::RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer )
{
	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->RegisterWithQuadRenderer( quadRenderer );
	}
}

void EveChildPlug::AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer ) const
{
	if (!m_display )
	{
		return;
	}
	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->AddQuadsToQuadRenderer( frustum, quadRenderer );
	}
}

void EveChildPlug::UpdateSyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params )
{
	EveChildUpdateParams newParams = params;
	newParams.isVisible &= m_display;
	newParams.childParent = this;

	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->UpdateSyncronous( updateContext, newParams );
	}
}

void EveChildPlug::UpdateAsyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params )
{
	Matrix localToWorldTransform = params.localToWorldTransform;
	if( params.childParent )
	{
		params.childParent->GetLocalToWorldTransform( localToWorldTransform );
	}
	else if( params.spaceObjectParent )
	{
		params.spaceObjectParent->GetLocalToWorldTransform( localToWorldTransform );
	}

	UpdateTransform( localToWorldTransform );

	EveChildUpdateParams newParams = params;
	newParams.isVisible &= m_display;
	newParams.childParent = this;

	for( auto it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		(*it)->UpdateAsyncronous( updateContext, newParams );
	}
}


void EveChildPlug::GetLocalToWorldTransform( Matrix& transform ) const
{
	transform = m_worldTransform;
}

void EveChildPlug::Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible )
{
	EveChildTransform::Setup( scale, rotation, translation, lowestLodVisible );
}

void EveChildPlug::GetDebugOptions( Tr2DebugRendererOptions& options )
{
	for( auto it = begin( m_objects ); it != end( m_objects ); ++it )
	{
		if( auto renderable = dynamic_cast<ITr2DebugRenderable*>( *it ) )
		{
			renderable->GetDebugOptions( options );
		}
	}
}

void EveChildPlug::RenderDebugInfo( Tr2DebugRenderer& renderer )
{
	if( !m_display )
	{
		return;
	}
	for( auto it = begin( m_objects ); it != end( m_objects ); ++it )
	{
		if( auto renderable = dynamic_cast<ITr2DebugRenderable*>( *it ) )
		{
			renderable->RenderDebugInfo( renderer );
		}
	}
}

IEveSpaceObjectChildPtr EveChildPlug::GetEffectChildByName( const char* name ) const
{
	for( auto it = begin( m_objects ); it != end( m_objects ); ++it )
	{
		auto child = *it;
		if( strcmp( child->GetName(), name ) == 0 )
		{
			return child;
		}
	}
	return nullptr;
}

void EveChildPlug::AddToEffectChildrenList( IEveSpaceObjectChild* child )
{
	m_objects.Append( child->GetRootObject() );
}

void EveChildPlug::RemoveFromEffectChildrenList( IEveSpaceObjectChild* child )
{
	auto index = m_objects.FindKey( child );
	if( index >= 0 )
	{
		m_objects.Remove( index );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Adds an external parameter to the external parameters list.
// --------------------------------------------------------------------------------
void EveChildPlug::AddExternalParameter( Tr2ExternalParameter* externalParameter )
{
	m_externalParameters.Append( externalParameter->GetRawRoot() );
}

const PTr2ExternalParameterVector& EveChildPlug::GetExternalParameters() const
{
	return m_externalParameters;
}
