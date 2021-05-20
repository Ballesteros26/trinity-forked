////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#include "StdAfx.h"
#include "Tr2ReflectionProbe.h"

BLUE_DEFINE( Tr2ReflectionProbe );

static const Be::VarChooser Tr2ReflectionProbe_HackBacklightApplication_Chooser[] = {
	{ "Plain", BeCast( Tr2ReflectionProbe::BACK_LIGHT_PLAIN ), "Add backlight directly to reflection" },
	{ "Modulate with background", BeCast( Tr2ReflectionProbe::BACK_LIGHT_MODULATE_WITH_BACKGROUND ), "Modulate backlight with background before applying" },
	{ "Modulate with cube map", BeCast( Tr2ReflectionProbe::BACK_LIGHT_MODULATE_WITH_CUBE_MAP ), "Modulate backlight with provided cube map before applying" },
	{ "Modulate with cube map (center)", BeCast( Tr2ReflectionProbe::BACK_LIGHT_MODULATE_WITH_CUBE_MAP_CENTER ), "Modulate backlight with provided cube map before applying (sampling at the center of backlight)" },
	{ 0 }
};

const Be::ClassInfo* Tr2ReflectionProbe::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ReflectionProbe, "" )
		MAP_INTERFACE( Tr2ReflectionProbe )
		MAP_INTERFACE( INotify )

		MAP_ATTRIBUTE( "unfilteredTexture", m_renderTargetCube, "Unfiltered reflection texture", Be::READ )
		MAP_ATTRIBUTE( "reflectionTexture", m_postFilterTarget, "Filtered reflection texture, with different roughness levels in mips", Be::READ )
		MAP_ATTRIBUTE( "position", m_position, "Origin for the reflection", Be::READWRITE )
		MAP_ATTRIBUTE( "reflectionSize", m_intermediateSize, "Size for the unfiltered reflection map", Be::READWRITE | Be::NOTIFY )
		MAP_ATTRIBUTE( "customSourceTexture", m_customSourceTexture, "A custom texture for filtering", Be::READWRITE | Be::NOTIFY)
		MAP_ATTRIBUTE( "hdrOutput", m_hdrOutput, "Generate HDR reflection texture", Be::READWRITE | Be::NOTIFY )

		MAP_ATTRIBUTE( "hackMode", m_hackMode, "Enables evil hacks, do not touch me\n:jessica-group: Hacks", Be::READWRITE | Be::NOTIFY )
		MAP_ATTRIBUTE( "intensity", m_intensity, "Reflection texture scale\n:jessica-group: Hacks", Be::READWRITE )
		MAP_ATTRIBUTE_WITH_CHOOSER( "backlightApplication", m_backlightApplication, "Back application method\n:jessica-group: Hacks", Be::READWRITE | Be::NOTIFY | Be::ENUM, Tr2ReflectionProbe_HackBacklightApplication_Chooser )
		MAP_ATTRIBUTE( "backlightColor", m_backlightColor, "Back light color\n:jessica-group: Hacks", Be::READWRITE )
		MAP_ATTRIBUTE( "backlightContrast", m_backlightContrast, "Back light shape/size\n:jessica-group: Hacks", Be::READWRITE )
		MAP_ATTRIBUTE( 
			"backlightCubeMapPath", 
			m_backlightModulateCubeMapPath, 
			"Path to the cubemap used to modulate backlight. Only applicable if backlightApplication is set to \"Modulate with cube map...\"\n"
			":jessica-group: Hacks", 
			Be::READWRITE | Be::NOTIFY )
		MAP_ATTRIBUTE(
			"backlightCubeMapMip",
			m_backlightModulateCubeMapMip,
			"Mip level of the cubemap used to modulate backlight. Only applicable if backlightApplication is set to \"Modulate with cube map...\"\n"
			":jessica-group: Hacks",
			Be::READWRITE )


		MAP_METHOD_AND_WRAP( "RunFilter", RunFilter, "Filters the currently set texture" )
	EXPOSURE_END()
}
