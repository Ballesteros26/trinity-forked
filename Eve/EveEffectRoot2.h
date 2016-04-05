#pragma once

#ifndef EveEffectRoot2_h
#define EveEffectRoot2_h

#include "include/ITriFunction.h"

#include "EveTransform.h"
#include "EveLODHelper.h"
#include "IEveSpaceObject2.h"
#include "Eve/SpaceObject/Children/IEveSpaceObjectChild.h"

BLUE_DECLARE( Tr2PointLight );
BLUE_DECLARE_VECTOR( Tr2PointLight );

BLUE_DECLARE( EveEffectRoot2 );

class EveEffectRoot2:
	public IEveSpaceObject2,
	public IInitialize
{
public:
    EXPOSE_TO_BLUE();
	using IEveSpaceObject2::Lock;
	using IEveSpaceObject2::Unlock;

	EveEffectRoot2( IRoot* lockobj = NULL );
	
	//////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize();

	/////////////////////////////////////////////////////////////////////////////////////
	// IEveSpaceObject2
	void UpdateSyncronous( EveUpdateContext& updateContext );
	void UpdateAsyncronous( EveUpdateContext& updateContext );
	void RenderDebugInfo( Tr2RenderContext& renderContext );
	void GetRenderables( const TriFrustum& frustum, std::vector<ITr2Renderable*>& renderables, const Matrix& parentTransform );
	bool GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query=EVE_BOUNDS_NORMAL ) const;
	void GetLights( Tr2LightManager& lightManager ) const;
	void GetPerObjectStructs( EveSpaceObjectVSData& vsData, EveSpaceObjectPSData& psData ) const;
	void UpdateModelCenterWorldPosition( Vector3 &position, Be::Time t );
	void GetModelCenterWorldPosition( Vector3 &position ) const;
	bool GetLocalBoundingBox( Vector3 &min, Vector3 &max );
	void GetLocalToWorldTransform( Matrix &transform ) const;

private:
	// general
	std::string m_name;
	bool m_display;

	void UpdateWorldTransform( Be::Time time );

	PIEveSpaceObjectChildVector m_effectChildren;


	Vector3 m_scaling;
	Quaternion m_rotation;
	Vector3 m_translation;

	Be::Time m_startTime;

	ITriVectorFunctionPtr m_ballPosition;
	ITriQuaternionFunctionPtr m_ballRotation;

	Vector4 m_boundingSphere;
	
	// last known results from updating m_ballPosition and m_ballRotation
	Matrix m_worldTransform;
	Matrix m_lastUpdateMatrix;
	Matrix m_localTransform;

	// PlacementObservers
	PTriObserverLocalVector m_observers;

	// Lods
	bool m_dynamicLODSelection;
	Tr2Lod m_lodLevel;
	
	PTr2PointLightVector m_lights;
	
	float GetBoundingSphereRadius() { return m_boundingSphere.w; }
	float m_estimatedSize;
	float m_effectDuration;
	
	IBlueEventListenerPtr m_loadedEventListener;

};

TYPEDEF_BLUECLASS( EveEffectRoot2 );

#endif // EveEffectRoot2_h