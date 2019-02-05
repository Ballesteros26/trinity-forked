////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#include "StdAfx.h"
#include "Tr2PPBloomEffect.h"
#include "Tr2Renderer.h"


Tr2PPBloomEffect::Tr2PPBloomEffect( IRoot* lockobj ) :
	m_luminanceThreshold( 0.0f ),
	m_luminanceScale( 0.5f ),
	m_bloomBrightness( 0.2f ),
	m_exposureDependency( false ),
	m_grimeWeight( 0.0f ),
	m_grimePath( "res:/texture/global/black.dds" )
{
	m_highPassFilter.CreateInstance();
	m_highPassFilter->StartUpdate();
	m_highPassFilter->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/HighPassFilter.fx" );
	m_highPassFilter->SetParameter( BlueSharedString( "LuminanceThreshold" ), m_luminanceThreshold );
	m_highPassFilter->SetParameter( BlueSharedString( "LuminanceScale" ), m_luminanceScale );
	m_highPassFilter->SetParameter( BlueSharedString( "ExposureDependency" ), m_exposureDependency );
	m_highPassFilter->EndUpdate();

	m_horizontalBlur.CreateInstance();
	m_horizontalBlur->StartUpdate();
	m_horizontalBlur->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/BlurBig.fx" );
	m_horizontalBlur->EndUpdate();

	m_verticalBlur.CreateInstance();
	m_verticalBlur->StartUpdate();
	m_verticalBlur->SetEffectPathName( "res:/Graphics/Effect/Managed/Space/PostProcess/BlurBig.fx" );
	m_verticalBlur->SetParameter( BlueSharedString( "Direction" ), Vector2( 0, 1 ) );
	m_verticalBlur->EndUpdate();
}

Tr2PPBloomEffect::~Tr2PPBloomEffect()
{

}

bool Tr2PPBloomEffect::Initialize()
{
	UpdateEffects();
	return true;
}

bool Tr2PPBloomEffect::OnModified( Be::Var* value )
{
	UpdateEffects();
	return true;
}

void Tr2PPBloomEffect::UpdateEffects() 
{
	m_highPassFilter->StartUpdate();
	m_highPassFilter->SetParameter( BlueSharedString( "LuminanceThreshold" ), m_luminanceThreshold );
	m_highPassFilter->SetParameter( BlueSharedString( "LuminanceScale" ), m_luminanceScale );
	m_highPassFilter->SetParameter( BlueSharedString( "ExposureDependency" ), m_exposureDependency );
	m_highPassFilter->EndUpdate();
}

void Tr2PPBloomEffect::Render( Tr2RenderContext& renderContext, Tr2PostProcessRenderInfo* renderInfo )
{
	if( !IsActive() )
	{
		return;
	}

	auto rt1 = renderInfo->GetRt1Buffer();
	auto rt2 = renderInfo->GetRt2Buffer();
	auto sourceCopy = renderInfo->GetSourceBufferCopy();
	auto exposureBuffer = renderInfo->GetExposureBuffer();

	Tr2Renderer::PushRenderTarget( *rt1, renderContext );
	
	HRESULT hr = renderContext.Clear( Tr2RenderContextEnum::CLEARFLAGS_TARGET, 0x00000000, 1.0, 0 );
	if( !SUCCEEDED( hr ) )
	{
		CCP_LOGERR( "Bloom RT1 clear failed" );
	}

	m_highPassFilter->StartUpdate();
	m_highPassFilter->SetParameter( BlueSharedString( "BlitCurrent" ), sourceCopy );
	m_highPassFilter->SetParameter( BlueSharedString( "Exposure" ), exposureBuffer );
	m_highPassFilter->EndUpdate();

	Tr2Renderer::DrawScreenQuad( m_highPassFilter );
	Tr2Renderer::PopRenderTarget( renderContext );

	Tr2Renderer::PushRenderTarget( *rt2, renderContext );
	 hr = renderContext.Clear( Tr2RenderContextEnum::CLEARFLAGS_TARGET, 0x00000000, 1.0, 0 );
	if( !SUCCEEDED( hr ) )
	{
		CCP_LOGERR( "Bloom RT2 clear failed" );
	}
	m_horizontalBlur->StartUpdate();
	m_horizontalBlur->SetParameter( BlueSharedString( "BlitCurrent" ), rt1 );
	m_horizontalBlur->EndUpdate();
	Tr2Renderer::DrawScreenQuad( m_horizontalBlur );
	Tr2Renderer::PopRenderTarget( renderContext );

	Tr2Renderer::PushRenderTarget( *rt1, renderContext );
	m_verticalBlur->StartUpdate();
	m_verticalBlur->SetParameter( BlueSharedString( "BlitCurrent" ), rt2 );
	m_verticalBlur->EndUpdate();

	Tr2Renderer::DrawScreenQuad( m_verticalBlur );
	Tr2Renderer::PopRenderTarget( renderContext );
}