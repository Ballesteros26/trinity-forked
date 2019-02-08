////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#pragma once

#ifndef Tr2PostProcessRenderInfo_H
#define Tr2PostProcessRenderInfo_H

#include "StdAfx.h"
#include "Shader\Tr2ShaderBuffer.h"
#include "Tr2RenderTarget.h"
#include "Tr2RenderContext.h"
#include "Tr2GpuBuffer.h"


BLUE_DECLARE( Tr2RenderTarget );
BLUE_DECLARE( Tr2RenderContext );
BLUE_DECLARE( Tr2ShaderBuffer );
BLUE_DECLARE( Tr2GpuBuffer );

BLUE_CLASS( Tr2PostProcessRenderInfo ) :
	public INotify
{
public:
	EXPOSE_TO_BLUE();

	Tr2PostProcessRenderInfo( IRoot* lockobj = NULL );
	~Tr2PostProcessRenderInfo();

	bool OnModified( Be::Var* value );

	void SetSourceBuffer( Tr2RenderTarget* sourceBuffer );
	Tr2RenderTarget* GetSourceBuffer() { return m_sourceBuffer; };
	Tr2RenderTarget* GetSourceBufferCopy() 
	{ 
		if( m_sourceBuffer && m_sourceBuffer->GetMsaaType() > 1 )
		{
			return m_sourceBufferCopy;
		}
		return m_sourceBuffer;		
	};

	Tr2RenderTarget* GetDestBuffer() { return m_destBuffer; };
	Tr2RenderTarget* GetVelocityBuffer() { return m_velocityBuffer; };
	Tr2RenderTarget* GetDistortionBuffer() { return m_distortionBuffer; };
	Tr2RenderTarget* GetAccumulationBuffer() { return m_accumulationBuffer; };
	Tr2RenderTarget* GetRt1Buffer() { return m_rt1; };
	Tr2RenderTarget* GetRt2Buffer() { return m_rt2; };
	Tr2RenderTarget* GetBlackBuffer() { return m_black; };

	Tr2ShaderBuffer* GetShaderBuffer() { return m_shaderBuffer; }
	
	Tr2GpuBuffer* GetExposureBuffer() { return m_exposure; }
	Tr2GpuBuffer* GetHistogramBuffer() { return m_histogram; }

	void SetPerFrameData( Tr2ShaderBuffer* psData );


private:
	void CopySourceTo( Tr2RenderTarget* buffer, float sizeScale );
	void CreateBuffers( uint32_t width, uint32_t height );
	void CreateHistogram( uint32_t width, uint32_t height );

	// Buffers
	Tr2RenderTargetPtr m_sourceBuffer;
	Tr2RenderTargetPtr m_destBuffer;
	Tr2RenderTargetPtr m_velocityBuffer;
	Tr2RenderTargetPtr m_distortionBuffer;
	Tr2RenderTargetPtr m_accumulationBuffer;
	Tr2RenderTargetPtr m_sourceBufferCopy;

	Tr2RenderTargetPtr m_rt1;
	Tr2RenderTargetPtr m_rt2;

	Tr2RenderTargetPtr m_black;

	Tr2ShaderBufferPtr m_shaderBuffer;

	Tr2GpuBufferPtr m_exposure;
	Tr2GpuBufferPtr m_histogram;

};

TYPEDEF_BLUECLASS( Tr2PostProcessRenderInfo );

#endif
