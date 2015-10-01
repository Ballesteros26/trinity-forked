////////////////////////////////////////////////////////////
//
//    Created:   September 2015
//    Copyright: CCP 2015
//

#pragma once
#ifndef EveImpactOverlay_H
#define EveImpactOverlay_H

#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "ITr2Renderable.h"
#include "ITr2GeometryProvider.h"
#include "Resources/Tr2LodResource.h"

BLUE_DECLARE( TriFrustum );
BLUE_DECLARE( Tr2MeshBase );
BLUE_DECLARE( EveUpdateContext );

BLUE_CLASS( EveImpactOverlay ) :
	public IInitialize,
	public Tr2DeviceResource
{
public:
	EXPOSE_TO_BLUE();

	EveImpactOverlay( IRoot* lockobj = NULL );
	~EveImpactOverlay();

	// shield impacts
	struct ShieldImpactData
	{
		Vector3 targetPosition;
		Vector3 interceptPosition;
		Vector3 direction;
		float timeLeft;
	};

	// shield texture data
	struct ShieldTexelData
	{
		Vector4 row0;
	};
	
	// armor impacts
	struct ArmorImpactData
	{
		Vector3 impactPosition;
		float timeLeft;
	};

	// armor texture data
	struct ArmorTexelData
	{
		Vector4 row0;
	};
	
	//////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize();

	/////////////////////////////////////////////////////////////
	// Tr2DeviceResource
	virtual void ReleaseResources( TriStorage s );
private:
	bool OnPrepareResources();
public:
#if TRINITYDEV
	void GetDescription( std::string& desc );
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	// Updates
	void UpdateSyncronous( EveUpdateContext& updateContext, EveSpaceObject2* parent );
	void UpdateAsyncronous( EveUpdateContext& updateContext, EveSpaceObject2* parent );
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Rendering
	void GetBatches( ITriRenderBatchAccumulator* accumulator, TriBatchType batchType, const Tr2PerObjectData* perObjectData );
	Tr2EffectPtr GetArmorDamageShader( TriBatchType batchType ) const;

	// control shield impacts
	int CreateShieldImpact( const Vector3& position, const Vector3& direction );
	bool GetShieldImpactPosition( Vector3& out, int shieldImpactIndex ) const;

	// control armor impacts
	int CreateArmorImpact( const Vector3& position );
	bool GetArmorImpactPosition( Vector3& out, int shieldImpactIndex ) const;

private:
	// general data
	BlueSharedString m_name;
	bool m_display;
	float m_speed;

	// limits
	uint32_t m_maxShieldImpacts;

	// parent object
	Vector3 m_shieldEllipsoidRadii;
	Vector3 m_shieldEllipsoidCenter;

	// a map of all shield impacts going on at the moment
	int m_shieldImpactDataNextIdx;
	std::map<int, ShieldImpactData> m_shieldImpactData;

	// a list of data used in the data texture
	std::vector<ShieldTexelData> m_shieldTexelData;

	// a map of all armor impacts going on at the moment
	int m_armorImpactDataNextIdx;
	std::map<int, ArmorImpactData> m_armorImpactData;

	// a list of data used in the data texture
	std::vector<ArmorTexelData> m_armorTexelData;

	// the data texture
	Tr2TextureAL m_shieldDataTexture;
	Tr2Variable m_shieldDataVar;

	// what to render
	Tr2MeshBasePtr m_mesh;

	// armor damage shader
	Tr2EffectPtr m_armorDamageShader;
};

TYPEDEF_BLUECLASS( EveImpactOverlay );

#endif