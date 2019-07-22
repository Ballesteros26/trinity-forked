////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//
#pragma once
#ifndef EveChildPlug_H
#define EveChildPlug_H

#include "IEveSpaceObjectChild.h"
#include "IEveEffectChildrenOwner.h"
#include "EveChildTransform.h"
#include "Tr2DebugRenderer.h"
#include "TransformModifiers/IEveChildTransformModifier.h"
#include "ITr2CurveSetOwner.h"
#include "ITr2SoundEmitterOwner.h"


BLUE_DECLARE( Tr2ExternalParameter );
BLUE_DECLARE_VECTOR( Tr2ExternalParameter );


BLUE_CLASS( EveChildPlug ) :
	public IEveSpaceObjectChild,
	public EveChildTransform,
	public IInitialize,
	public IEveEffectChildrenOwner,
	public ITr2DebugRenderable
{
public:
	EXPOSE_TO_BLUE();

	EveChildPlug( IRoot* lockobj = NULL );
	~EveChildPlug();

	//////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize();

	/////////////////////////////////////////////////////////////////////////////////////
	// IEveEffectChildrenOwner
	IEveSpaceObjectChildPtr GetEffectChildByName( const char* name ) const;
	void AddToEffectChildrenList( IEveSpaceObjectChild* child );
	void RemoveFromEffectChildrenList( IEveSpaceObjectChild* child );

	const char* GetName() const;
	void SetName( const char* name );

	void UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform, Tr2Lod parentLod );
	void GetRenderables( std::vector<ITr2Renderable*>& renderables );
	bool GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query=EVE_BOUNDS_NORMAL ) const;
	void RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer );
	void AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer ) const;
	
	void UpdateSyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params );
	void UpdateAsyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params );
	void UpdateAsyncronous( EveUpdateContext& updateContext, Matrix& parentTransform );
	void GetLocalToWorldTransform( Matrix& transform ) const;

	void Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible );

	void GetDebugOptions( Tr2DebugRendererOptions& options );
	void RenderDebugInfo( Tr2DebugRenderer& renderer );

	// external parameters
	void AddExternalParameter( Tr2ExternalParameter* externalParameter );
	const PTr2ExternalParameterVector& GetExternalParameters() const;

	/////////////////////////////////////////////////////////////////////////////////////
	// IEveSpaceObjectChild
	void ChangeLOD( Tr2Lod lod ) {};
	void GetLights( Tr2LightManager& lightManager ) const {};

	PIEveSpaceObjectChildVector m_objects;
protected:
	BlueSharedString m_name;

	PTr2ExternalParameterVector m_externalParameters;

	bool m_display;
};

TYPEDEF_BLUECLASS( EveChildPlug );

#endif