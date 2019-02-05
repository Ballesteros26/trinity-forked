////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#pragma once
#include "StdAfx.h"
#include "Tr2PostProcess2.h"
#include "Tr2Renderer.h"
#include "Tr2PostProcessRenderInfo.h"


Tr2PostProcess2::Tr2PostProcess2( IRoot* lockobj )
{
	m_tonemappingEffect.CreateInstance();
	m_tonemappingEffect->StartUpdate();
	m_tonemappingEffect->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/ToneMapping.fx" );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Method" ), 3.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteDetailScroll" ), Vector4( 0.0, 0.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainColorAmount" ), 0.600000023842 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Tonemapping" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "Desaturate" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteColor" ), Vector4( 1.0, 1.0, 1.0, 1.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteSineRange" ), Vector4( 0.0, 1.0, 0.0, 0.0 ) );
	m_tonemappingEffect->SetParameter( BlueSharedString( "MaxExposure" ), 10.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainIntensity" ), 0.00300000002608 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ColoredGrain" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "LUTEnabled" ), 1.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "FadeAmount" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrimeWeight" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "ExposureAdjust" ), 2.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "DynamicExposure" ), 0.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "GrainSize" ), 2.0 );
	m_tonemappingEffect->SetParameter( BlueSharedString( "VignetteEnabled" ), 1.0 );
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


Tr2PostProcess2::~Tr2PostProcess2()
{
}


void Tr2PostProcess2::Render( Tr2RenderContext& renderContext, Tr2PostProcessRenderInfo* renderInfo )
{
	auto sourceBuffer = renderInfo->GetSourceBuffer();

	if( !sourceBuffer )
	{
		return;
	}

	// copy the source to the source copy buffer
	if( sourceBuffer->GetMsaaType() > 1 )
	{
		sourceBuffer->GetRenderTarget().Resolve( *renderInfo->GetSourceBufferCopy(), renderContext );
	}

	Tr2RenderTarget* blitCurrent = renderInfo->GetBlackBuffer();

	renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_FULLSCREEN );

	Tr2Renderer::PushRenderTarget( *sourceBuffer, renderContext );
	Tr2Renderer::PushDepthStencilBuffer( nullDS, renderContext );
		
	renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_ALPHA_ADDITIVE );
	if( m_godRays && m_godRays->IsActive() )
	{
		m_godRays->Render( renderContext, renderInfo );
	}
	
	if( m_bloom && m_bloom->IsActive() )
	{
		renderContext.m_esm.ApplyStandardStates( Tr2EffectStateManager::RM_ALPHA_ADDITIVE );
		m_bloom->Render( renderContext, renderInfo );
		blitCurrent = renderInfo->GetRt1Buffer();
	}

	m_tonemappingEffect->StartUpdate();
	m_tonemappingEffect->SetParameter( BlueSharedString( "BlitOriginal" ), renderInfo->GetSourceBufferCopy() );
	m_tonemappingEffect->SetParameter( BlueSharedString( "BlitCurrent" ), blitCurrent );
	m_tonemappingEffect->EndUpdate();
	Tr2Renderer::DrawScreenQuad( m_tonemappingEffect );

	if( m_signalLoss && m_signalLoss->IsActive() )
	{
		m_signalLoss->Render( renderContext, renderInfo );
	}


	Tr2Renderer::PopDepthStencilBuffer( renderContext );
	Tr2Renderer::PopRenderTarget( renderContext );
}


