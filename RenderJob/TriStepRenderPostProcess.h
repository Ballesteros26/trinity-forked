////////////////////////////////////////////////////////////
//
//    Created:   February 2019
//    Copyright: CCP 2019
//

#pragma once
#ifndef TriStepRenderPostProcess_H
#define TriStepRenderPostProcess_H

#include "TriRenderStep.h"
#include "Eve/EveSpaceScene.h"
#include "Tr2RenderTarget.h"
#include "PostProcess/Tr2PostProcessRenderInfo.h"


// -------------------------------------------------------------
// Description:
//   A render step to render post process. Takes a scene and the source buffer as a parameter
// SeeAlso:
//   TriRenderStep
// -------------------------------------------------------------
BLUE_CLASS( TriStepRenderPostProcess ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();

	TriStepRenderPostProcess( IRoot* lockobj = 0 );
	~TriStepRenderPostProcess( void );

	//RenderStep
	TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext );

	void py__init__( EveSpaceScene* scene, Tr2RenderTarget* source, Tr2ShaderBuffer* psData );
private:
	void PushRenderTarget( Tr2RenderContext& renderContext, Tr2RenderTarget* rt = nullptr );

	// bloom
	bool ProcessBloom( Tr2PPBloomEffect* bloom );
	void RenderBloom( Tr2RenderContext& renderContext, Tr2PPBloomEffect* bloom );
	Tr2EffectPtr m_bloomHighPassFilter;
	Tr2EffectPtr m_bloomHorizontalBlur;
	Tr2EffectPtr m_bloomVerticalBlur;

	// godRays
	bool TriStepRenderPostProcess::ProcessGodRays( Tr2PPGodRaysEffect* godrays );
	void TriStepRenderPostProcess::RenderGodRays( Tr2RenderContext& renderContext, Tr2PPGodRaysEffect* godrays );
	Tr2EffectPtr m_godRayDownSampleEffect;
	Tr2EffectPtr m_godrayEffect;

	// signal loss
	bool TriStepRenderPostProcess::ProcessSignalLoss( Tr2PPSignalLossEffect* signalLoss );
	void TriStepRenderPostProcess::RenderSignalLoss( Tr2RenderContext& renderContext, Tr2PPSignalLossEffect* godrays );
	Tr2EffectPtr m_signalLossEffect;


	// tonemapping
	Tr2EffectPtr m_tonemappingEffect;

	EveSpaceScenePtr m_scene;
	Tr2PostProcessRenderInfoPtr m_renderInfo;
};

TYPEDEF_BLUECLASS( TriStepRenderPostProcess );

#endif // TriStepRenderPostProcess_H