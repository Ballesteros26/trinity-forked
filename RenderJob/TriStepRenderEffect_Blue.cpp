#include "StdAfx.h"
#include "TriStepRenderEffect.h"
#include "Tr2Effect.h"
#include "Tr2ShaderBuffer.h"

BLUE_DEFINE( TriStepRenderEffect );

const Be::ClassInfo* TriStepRenderEffect::ExposeToBlue()
{
	EXPOSURE_BEGIN(TriStepRenderEffect, "" )

		MAP_INTERFACE( TriRenderStep )
		MAP_INTERFACE( TriStepRenderEffect )

		MAP_ATTRIBUTE( "effect", m_effect, "na", Be::READWRITE )
		MAP_ATTRIBUTE( "shaderBuffer", m_shaderBuffer, "additional shader data", Be::READWRITE )
		MAP_ATTRIBUTE( "tlTexCoord", m_tlTexCoord, "top left texture coordinate", Be::READWRITE )
		MAP_ATTRIBUTE( "brTexCoord", m_brTexCoord, "bottom right texture coordinate", Be::READWRITE )

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS
		( 
			"__init__", 
			py__init__,
			2,
			"Create a render step that draws a fullscreen quad using a specified effect"
			"\n"
			"\nOptional Arguments:"
			"\neffect - A Tr2Effect (default None)"
			"\nshaderBuffer - A Tr2ShaderBuffer (default None)"
		)

	EXPOSURE_CHAINTO( TriRenderStep )
}