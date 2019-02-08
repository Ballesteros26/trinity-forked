////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#include "StdAfx.h"
#include "Tr2PostProcessRenderInfo.h"
#include "Tr2RenderTarget.h"

BLUE_DEFINE( Tr2PostProcessRenderInfo );

const Be::ClassInfo* Tr2PostProcessRenderInfo::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2PostProcessRenderInfo, "" )
		MAP_INTERFACE( INotify )


		MAP_ATTRIBUTE( "destBuffer", m_destBuffer, "The destination buffer of the post process", Be::READWRITE )
		MAP_ATTRIBUTE( "sourceBuffer", m_sourceBuffer, "The source buffer of the post process", Be::READWRITE | Be::NOTIFY )
		MAP_ATTRIBUTE( "sourceBufferCopy", m_sourceBufferCopy, "A copy of the source buffer, before any post processing is rendered", Be::READ )
		MAP_ATTRIBUTE( "velocityBuffer", m_velocityBuffer, "The velocity buffer of the post process", Be::READ )
		MAP_ATTRIBUTE( "accumulationBuffer", m_accumulationBuffer, "The accumulation buffer of the post process", Be::READ )

		MAP_ATTRIBUTE( "rt1", m_rt1, "One of the render buffers", Be::READ )
		MAP_ATTRIBUTE( "rt2", m_rt2, "the other render buffer", Be::READ )
		
		EXPOSURE_END()
}

