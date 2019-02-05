////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#pragma once
#ifndef Tr2PPBloomEffect_H
#define Tr2PPBloomEffect_H

#include "StdAfx.h"
#include "Shader/Tr2Effect.h"
#include "PostProcess/Effects/Tr2PPEffect.h"
#include "PostProcess/Tr2PostProcessRenderInfo.h"


BLUE_CLASS( Tr2PPBloomEffect ) :
	public Tr2PPEffect,
	public IInitialize,
	public INotify
{
public:
	EXPOSE_TO_BLUE();

	Tr2PPBloomEffect( IRoot* lockobj = NULL );
	~Tr2PPBloomEffect();

	void Render( Tr2RenderContext& renderContext, Tr2PostProcessRenderInfo* renderInfo );


	//////////////////////////////////////////////////////////////////////////
	// IInitialize
	virtual bool Initialize() override;

	//////////////////////////////////////////////////////////////////////////
	// INotify
	virtual bool OnModified( Be::Var* value ) override;
		

private:
	void UpdateEffects();

	float m_luminanceThreshold;
	float m_luminanceScale;
	float m_bloomBrightness;
	bool m_exposureDependency;
	float m_grimeWeight;
	BlueSharedString m_grimePath;

	Tr2EffectPtr m_highPassFilter;
	Tr2EffectPtr m_horizontalBlur;
	Tr2EffectPtr m_verticalBlur;
};

TYPEDEF_BLUECLASS( Tr2PPBloomEffect );

#endif