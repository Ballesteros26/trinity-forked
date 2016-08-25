////////////////////////////////////////////////////////////
//
//    Created:   August 2016
//    Copyright: CCP 2016
//
#include "StdAfx.h"
#include "EveChildQuad.h"

BLUE_DEFINE( EveChildQuad );

const Be::ClassInfo* EveChildQuad::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveChildQuad, "" )
        MAP_INTERFACE( EveChildQuad )
		MAP_INTERFACE( IEveSpaceObjectChild )
		MAP_INTERFACE( IInitialize )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST	)
		MAP_ATTRIBUTE( "display", m_display, "Show/hide object", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "editMode", m_editMode, "Turn on when tweaking effect", Be::READWRITE )
		MAP_ATTRIBUTE( "effect", m_effect, "Effect to be used for rendering a quad", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "color", m_color, "Quad color", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "brightness", m_brightness, "Quad brightness", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE( "position", m_position, "Quad position", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "rotation", m_rotation, "Quad rotation", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "scale", m_scale, "Quad scale", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}