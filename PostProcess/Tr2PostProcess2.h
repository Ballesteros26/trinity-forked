////////////////////////////////////////////////////////////////////////////////
//
// Created:	January 2019
// Copyright:	CCP 2019
//

#pragma once
#ifndef Tr2PostProcess2_H
#define Tr2PostProcess2_H

#include "Tr2PostProcessRenderInfo.h"
#include "Shader/Tr2Effect.h"
#include "Effects/Tr2PPSignalLossEffect.h"
#include "Effects/Tr2PPGodRaysEffect.h"
#include "Effects/Tr2PPBloomEffect.h"

BLUE_DECLARE( Tr2Effect );
BLUE_DECLARE( Tr2PPSignalLossEffect );
BLUE_DECLARE( Tr2PPGodRaysEffect );
BLUE_DECLARE( Tr2PPBloomEffect );

BLUE_CLASS( Tr2PostProcess2 ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();

	Tr2PostProcess2( IRoot* lockobj = NULL );
	~Tr2PostProcess2();

	void Render( Tr2RenderContext& renderContext, Tr2PostProcessRenderInfo* renderInfo );

private:
	Tr2PPSignalLossEffectPtr m_signalLoss;
	Tr2PPGodRaysEffectPtr m_godRays;
	Tr2PPBloomEffectPtr m_bloom;

	Tr2EffectPtr m_tonemappingEffect;

};
TYPEDEF_BLUECLASS( Tr2PostProcess2 );

#endif // Tr2PostProcess_H
