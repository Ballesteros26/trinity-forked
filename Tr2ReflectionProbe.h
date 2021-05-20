////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#pragma once

#include "Tr2DeviceResource.h"
#include <array>

BLUE_DECLARE( Tr2Effect );
BLUE_DECLARE( Tr2TextureReference );
BLUE_DECLARE( Tr2RenderTarget );
BLUE_DECLARE( TriTextureRes );
BLUE_DECLARE_INTERFACE( ITriTextureRes );

BLUE_CLASS( Tr2ReflectionProbe ) :
	public INotify,
	public Tr2DeviceResource
{
public:
	EXPOSE_TO_BLUE();

	enum HackBacklightApplication
	{
		BACK_LIGHT_PLAIN,
		BACK_LIGHT_MODULATE_WITH_BACKGROUND,
		BACK_LIGHT_MODULATE_WITH_CUBE_MAP,
		BACK_LIGHT_MODULATE_WITH_CUBE_MAP_CENTER,
	};

	Tr2ReflectionProbe( IRoot* lockobj = NULL );
	~Tr2ReflectionProbe();

	void ReleaseResources( TriStorage s );
	bool OnPrepareResources();

	bool IsValid();
	void InitRenderPass( Tr2RenderContext &renderContext );
	void StartRenderFace( unsigned face, Tr2RenderContext &renderContext );
	void EndRenderPass( Tr2RenderContext &renderContext );

	Tr2RenderTargetPtr GetReflection();
	void SetPosition( Vector3 position );

	bool OnModified( Be::Var* value );

private:
	void RunFilter();
	void Filter( Tr2RenderContext &renderContext );

	bool m_initialized;
	Vector3 m_position;
	int m_intermediateSize;

	std::array<Tr2RenderTargetPtr, 6> m_renderTargets;
	std::array<Tr2TextureAL, 6> m_stencilMaps;
	Tr2RenderTargetPtr m_renderTargetCube;

	Tr2EffectPtr m_preFilterEffect;
	Tr2EffectPtr m_filterEffect;
	Tr2RenderTargetPtr m_preFilterTarget;
	Tr2RenderTargetPtr m_postFilterTarget;

	ITriTextureResPtr m_customSourceTexture;

	bool m_prevCullInversion;
	bool m_hdrOutput;

	// TEMPORARY controls for experiments with environment maps
	bool m_hackMode;
	HackBacklightApplication m_backlightApplication;
	Color m_backlightColor;
	float m_backlightContrast;
	float m_intensity;
	std::wstring m_backlightModulateCubeMapPath;
	TriTextureResPtr m_backlightModulateCubeMap;
	uint32_t m_backlightModulateCubeMapMip;
};

TYPEDEF_BLUECLASS( Tr2ReflectionProbe );