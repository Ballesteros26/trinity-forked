#include "StdAfx.h"
#include "TriStepRenderPostProcess.h"

BLUE_DEFINE( TriStepRenderPostProcess );

const Be::ClassInfo* TriStepRenderPostProcess::ExposeToBlue()
{
	EXPOSURE_BEGIN( TriStepRenderPostProcess, "Render step for rendering post process" )

		MAP_INTERFACE( TriRenderStep )
		MAP_INTERFACE( TriStepRenderPostProcess )

		MAP_ATTRIBUTE( "scene", m_scene, "The scene to be rendered", Be::READWRITE )
		MAP_ATTRIBUTE( "renderInfo", m_renderInfo, "The rendering information for the post process", Be::READWRITE )
		MAP_ATTRIBUTE( "bloomHighPassFilter", m_bloomHighPassFilter, "The bloom high pass effect", Be::READWRITE )
		MAP_ATTRIBUTE( "bloomHorizontalBlur", m_bloomHorizontalBlur, "The bloom horizontal blur effect", Be::READWRITE )
		MAP_ATTRIBUTE( "bloomVerticalBlur", m_bloomVerticalBlur, "The bloom vertical blur", Be::READWRITE )
		MAP_ATTRIBUTE( "godRayDownSampleEffect", m_godRayDownSampleEffect, "The godray downsample effect", Be::READWRITE )
		MAP_ATTRIBUTE( "godrayEffect", m_godrayEffect, "The godray effect", Be::READWRITE )
		MAP_ATTRIBUTE( "signalLossEffect", m_signalLossEffect, "The signal loss effect", Be::READWRITE )
		MAP_ATTRIBUTE( "tonemappingEffect", m_tonemappingEffect, "The tone mapping effect", Be::READWRITE )

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS
		(
			"__init__",
			py__init__,
			2,
			"Creates a render step that renders post processes\n"
			":param scene: an ITr2Scene object\n"
			":param source: an Tr2RenderTarget object"
		)



		EXPOSURE_CHAINTO( TriRenderStep )
}