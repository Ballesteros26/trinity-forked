#include "StdAfx.h"
#include "TriStepRenderPostProcess.h"
#include "PostProcess/Tr2PostProcess2.h"
#include "Shader/Parameter/TriTextureParameter.h"


TriStepRenderPostProcess::TriStepRenderPostProcess( IRoot* lockobj )
{
	m_renderInfo.CreateInstance();
	m_tonemappingEffect.CreateInstance();
	m_tonemappingEffect->StartUpdate();
	m_tonemappingEffect->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/ToneMapping.fx" );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Method" ), 3.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteDetailScroll" ), Vector4( 0.0, 0.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainColorAmount" ), 0.600000023842 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Tonemapping" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Desaturate" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteColor" ), Vector4( 1.0, 1.0, 1.0, 1.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteSineRange" ), Vector4( 0.0, 1.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "MaxExposure" ), 10.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainIntensity" ), 0.00300000002608 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ColoredGrain" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "LUTEnabled" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "FadeAmount" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrimeWeight" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ExposureAdjust" ), 2.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "DynamicExposure" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainSize" ), 2.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteEnabled" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainLuminanceExponent" ), 0.20000000298 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "FadeColor" ), Vector4( 0.0, 0.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "FilmGrain" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ExposureMiddleValue" ), 0.5 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteDetailSize" ), Vector4( 16.0, 16.0, 16.0, 16.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "LUTInfluence" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteSineFrequency" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ExposureInfluence" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "BloomBrightness" ), 0.20000000298 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteIntensity" ), Vector4( 0.0, 0.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "MinExposure" ), -1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "SaturationFactor" ), 1.0 );

	m_tonemappingEffect->EndUpdate();
}

TriStepRenderPostProcess::~TriStepRenderPostProcess( void )
{
}

void TriStepRenderPostProcess::py__init__( EveSpaceScene* scene, Tr2RenderTarget* source, Tr2ShaderBuffer* psData  )
{
	m_scene = scene;
	m_renderInfo->SetSourceBuffer( source );
	m_renderInfo->SetPerFrameData( psData );
}


void TriStepRenderPostProcess::PushRenderTarget( Tr2RenderContext& renderContext, Tr2RenderTarget* rt)
{
	if( rt != nullptr )
	{
		Tr2Renderer::PushRenderTarget( *rt, renderContext );
	}
	else if( m_renderInfo->GetDestBuffer() )
	{
		Tr2Renderer::PushRenderTarget( *m_renderInfo->GetDestBuffer(), renderContext );
	}
	else
	{
		Tr2Renderer::PushRenderTarget( renderContext );
	}
}


TriStepResult TriStepRenderPostProcess::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	auto sourceBuffer = m_renderInfo->GetSourceBuffer();

	if( !sourceBuffer )
	{
		return RS_OK;
	}

	Tr2PostProcess2Ptr postProcess = m_scene->GetPostProcess();

	Tr2PPGodRaysEffect* godrays = nullptr;
	Tr2PPBloomEffect* bloom = nullptr;
	Tr2PPSignalLossEffect* signalLoss= nullptr;

	if( postProcess != nullptr )
	{
		godrays = postProcess->GetGodRays();
		bloom = postProcess->GetBloom();
		signalLoss = postProcess->GetSignalLoss();
	}

	Tr2RenderTarget* blitCurrent = m_renderInfo->GetBlackBuffer();

	renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_FULLSCREEN );
	
	PushRenderTarget( renderContext );
	Tr2Renderer::PushDepthStencilBuffer( nullDS, renderContext );

	if( ProcessGodRays( godrays ) )
	{
		RenderGodRays( renderContext, godrays );
	}
	
	renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_FULLSCREEN );

	// copy the source to the source copy buffer
	if( sourceBuffer->GetMsaaType() > 1 )
	{
		sourceBuffer->GetRenderTarget().Resolve( *m_renderInfo->GetSourceBufferCopy(), renderContext );
	}

	if( ProcessBloom( bloom) )
	{
		RenderBloom( renderContext, bloom );
		blitCurrent = m_renderInfo->GetRt1Buffer();
	}

	m_tonemappingEffect->StartUpdate();
	m_tonemappingEffect->SetParameter( BlueSharedString( "BlitOriginal" ), m_renderInfo->GetSourceBufferCopy() );
	m_tonemappingEffect->SetParameter( BlueSharedString( "BlitCurrent" ), blitCurrent );
	m_tonemappingEffect->EndUpdate();

	Tr2Renderer::DrawTexture( m_tonemappingEffect, Vector2( 0, 0 ), Vector2( 1, 1 ) );
	
	if( ProcessSignalLoss( signalLoss ) )
	{
		RenderSignalLoss( renderContext, signalLoss );
	}

	Tr2Renderer::PopDepthStencilBuffer( renderContext );
	Tr2Renderer::PopRenderTarget( renderContext );

	return RS_OK;
}


bool TriStepRenderPostProcess::ProcessBloom( Tr2PPBloomEffect* bloom )
{
	if( bloom && ( m_bloomHighPassFilter == nullptr || m_bloomHorizontalBlur == nullptr || m_bloomVerticalBlur == nullptr ) )
	{
		m_bloomHighPassFilter.CreateInstance();
		m_bloomHighPassFilter->StartUpdate();
		m_bloomHighPassFilter->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/HighPassFilter.fx" );
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "LuminanceThreshold" ), bloom->m_luminanceThreshold );
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "LuminanceScale" ), bloom->m_luminanceScale );
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "ExposureDependency" ), bloom->m_exposureDependency );
		m_bloomHighPassFilter->EndUpdate();

		m_bloomHorizontalBlur.CreateInstance();
		m_bloomHorizontalBlur->StartUpdate();
		m_bloomHorizontalBlur->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/BlurBig.fx" );
		m_bloomHorizontalBlur->EndUpdate();

		m_bloomVerticalBlur.CreateInstance();
		m_bloomVerticalBlur->StartUpdate();
		m_bloomVerticalBlur->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/BlurBig.fx" );
		m_bloomVerticalBlur->SetParameter( BlueSharedString( "Direction" ), Vector2( 0, 1 ) );
		m_bloomVerticalBlur->EndUpdate();

		bloom->SetDirty( false );

	}
	else if( bloom == nullptr )
	{
		if( m_bloomHighPassFilter != nullptr || m_bloomHorizontalBlur != nullptr || m_bloomVerticalBlur != nullptr )
		{
			m_bloomHighPassFilter = nullptr;
			m_bloomHorizontalBlur = nullptr;
			m_bloomVerticalBlur = nullptr;
		}
	}

	if( bloom && bloom->IsDirty() )
	{
		m_bloomHighPassFilter->StartUpdate();
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "LuminanceThreshold" ), bloom->m_luminanceThreshold );
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "LuminanceScale" ), bloom->m_luminanceScale );
		m_bloomHighPassFilter->SetParameter( BlueSharedString( "ExposureDependency" ), bloom->m_exposureDependency );
		m_bloomHighPassFilter->EndUpdate();
		bloom->SetDirty( false );
	}

	return bloom != nullptr && bloom->IsActive();
}

void TriStepRenderPostProcess::RenderBloom( Tr2RenderContext& renderContext, Tr2PPBloomEffect* bloom )
{
	if( !bloom->IsActive() )
	{
		return;
	}
	auto rt1 = m_renderInfo->GetRt1Buffer();
	auto rt2 = m_renderInfo->GetRt2Buffer();
	auto sourceCopy = m_renderInfo->GetSourceBufferCopy();
	auto exposureBuffer = m_renderInfo->GetExposureBuffer();

	Tr2Renderer::PushRenderTarget( *rt1, renderContext );

	HRESULT hr = renderContext.Clear( Tr2RenderContextEnum::CLEARFLAGS_TARGET, 0x00000000, 1.0, 0 );
	if( !SUCCEEDED( hr ) )
	{
		CCP_LOGERR( "Bloom RT1 clear failed" );
	}

	m_bloomHighPassFilter->StartUpdate();
	m_bloomHighPassFilter->SetParameter( BlueSharedString( "BlitCurrent" ), sourceCopy );
	m_bloomHighPassFilter->SetParameter( BlueSharedString( "Exposure" ), exposureBuffer );
	m_bloomHighPassFilter->EndUpdate();

	Tr2Renderer::DrawScreenQuad( m_bloomHighPassFilter );
	Tr2Renderer::PopRenderTarget( renderContext );

	Tr2Renderer::PushRenderTarget( *rt2, renderContext );
	hr = renderContext.Clear( Tr2RenderContextEnum::CLEARFLAGS_TARGET, 0x00000000, 1.0, 0 );
	if( !SUCCEEDED( hr ) )
	{
		CCP_LOGERR( "Bloom RT2 clear failed" );
	}
	m_bloomHorizontalBlur->StartUpdate();
	m_bloomHorizontalBlur->SetParameter( BlueSharedString( "BlitCurrent" ), rt1 );
	m_bloomHorizontalBlur->EndUpdate();
	Tr2Renderer::DrawScreenQuad( m_bloomHorizontalBlur );
	Tr2Renderer::PopRenderTarget( renderContext );

	Tr2Renderer::PushRenderTarget( *rt1, renderContext );
	m_bloomVerticalBlur->StartUpdate();
	m_bloomVerticalBlur->SetParameter( BlueSharedString( "BlitCurrent" ), rt2 );
	m_bloomVerticalBlur->EndUpdate();

	Tr2Renderer::DrawScreenQuad( m_bloomVerticalBlur );
	Tr2Renderer::PopRenderTarget( renderContext );
}


bool TriStepRenderPostProcess::ProcessGodRays( Tr2PPGodRaysEffect* godrays )
{
	if( godrays && ( m_godRayDownSampleEffect == nullptr || m_godrayEffect == nullptr ) )
	{
		m_godRayDownSampleEffect.CreateInstance();
		m_godRayDownSampleEffect->StartUpdate();
		m_godRayDownSampleEffect->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/DownsampleDepth.fx" );
		m_godRayDownSampleEffect->EndUpdate();

		m_godrayEffect.CreateInstance();
		m_godrayEffect->StartUpdate();
		m_godrayEffect->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/Godrays.fx" );
		m_godrayEffect->SetParameter( BlueSharedString( "Color" ), godrays->m_godRayColor );
		m_godrayEffect->SetParameter( BlueSharedString( "Intensity" ), Vector4(godrays->m_intensity, 0.0f, 1.0f, 1.0f ) );
		m_godrayEffect->SetParameter( BlueSharedString( "grFactors" ), godrays->grFactors );
		m_godrayEffect->AddResourceTexture2D( BlueSharedString( "NoiseTexMap" ), godrays->m_noiseTexturePath.c_str() );
		m_godrayEffect->EndUpdate();
		godrays->SetDirty( false );
	}
	else if( godrays == nullptr )
	{
		if( m_bloomHighPassFilter != nullptr || m_bloomHorizontalBlur != nullptr || m_bloomVerticalBlur != nullptr )
		{
			m_bloomHighPassFilter = nullptr;
			m_bloomHorizontalBlur = nullptr;
			m_bloomVerticalBlur = nullptr;
		}
	}

	if( godrays && godrays->IsDirty() )
	{
		m_godrayEffect->StartUpdate();
		m_godrayEffect->SetParameter( BlueSharedString( "Color" ), godrays->m_godRayColor );
		m_godrayEffect->SetParameter( BlueSharedString( "Intensity" ), Vector4( godrays->m_intensity, 0.0f, 1.0f, 1.0f ) );
		m_godrayEffect->SetParameter( BlueSharedString( "grFactors" ), godrays->grFactors );

		TriTextureParameter* resource = dynamic_cast< TriTextureParameter* >( m_godrayEffect->GetResourceByName( "NoiseTexMap" ) );
		resource->SetResourcePath( godrays->m_noiseTexturePath.c_str() );

		m_godrayEffect->EndUpdate();
		godrays->SetDirty( false );
	}

	return godrays != nullptr && godrays->IsActive();
}


void TriStepRenderPostProcess::RenderGodRays( Tr2RenderContext& renderContext, Tr2PPGodRaysEffect* godrays)
{
	auto shaderBuffer = m_renderInfo->GetShaderBuffer();
	auto rt1 = m_renderInfo->GetRt1Buffer();
	auto rt2 = m_renderInfo->GetRt2Buffer();
	auto backBufferRT = m_renderInfo->GetSourceBuffer();

	// Downsample 
	Tr2Renderer::PushRenderTarget( *rt1, renderContext );
	Tr2Renderer::DrawScreenQuad( m_godRayDownSampleEffect );
	Tr2Renderer::PopRenderTarget( renderContext );

	// God rays
	Tr2Renderer::PushRenderTarget( *rt2, renderContext );
	HRESULT hr = renderContext.Clear( Tr2RenderContextEnum::CLEARFLAGS_TARGET, 0x00000000, 1.0, 0 );

	if( !SUCCEEDED( hr ) )
	{
		CCP_LOGERR( "Godray clear failed" );
	}
	m_godrayEffect->StartUpdate();
	m_godrayEffect->SetParameter( BlueSharedString( "DepthMap" ), rt1 );
	m_godrayEffect->EndUpdate();

	Tr2Renderer::DrawScreenQuad( m_godrayEffect );
	Tr2Renderer::PopRenderTarget( renderContext );

	renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_ALPHA_ADDITIVE );
	// Blit
	Tr2Renderer::PushRenderTarget( *backBufferRT, renderContext );
	Tr2Renderer::DrawTexture( *rt2, Vector2( 0, 0 ), Vector2( 1, 1 ) );
	Tr2Renderer::PopRenderTarget( renderContext );
}


bool TriStepRenderPostProcess::ProcessSignalLoss( Tr2PPSignalLossEffect* signalLoss )
{
	if( signalLoss && m_signalLossEffect == nullptr )
	{
		m_signalLossEffect.CreateInstance();
		m_signalLossEffect->StartUpdate();
		m_signalLossEffect->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/SignalLoss.fx" );
		m_signalLossEffect->SetParameter( BlueSharedString( "NoiseStrength" ), signalLoss->m_strength );
		m_signalLossEffect->EndUpdate();
		signalLoss->SetDirty( false );
	}
	else if( signalLoss == nullptr )
	{
		if( m_signalLossEffect != nullptr )
		{
			m_signalLossEffect = nullptr;
		}
	}

	if( signalLoss && signalLoss->IsDirty() )
	{
		m_signalLossEffect->StartUpdate();
		m_signalLossEffect->SetParameter( BlueSharedString( "NoiseStrength" ), signalLoss->m_strength );
		m_signalLossEffect->EndUpdate();
		signalLoss->SetDirty( false );
	}
	return signalLoss != nullptr && signalLoss->IsActive();
}


void TriStepRenderPostProcess::RenderSignalLoss( Tr2RenderContext& renderContext, Tr2PPSignalLossEffect* signalLoss )
{
	PushRenderTarget( renderContext );
	Tr2Renderer::DrawScreenQuad( m_signalLossEffect );
	Tr2Renderer::PopRenderTarget( renderContext );

}