#include "StdAfx.h"
#include "EveStretch.h"
#include "TriFloat.h"

BLUE_DEFINE( EveStretch );

const Be::ClassInfo* EveStretch::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveStretch, "" )
        MAP_INTERFACE( EveStretch )
		MAP_INTERFACE( IEveTransform )
		MAP_INTERFACE( IEveSpaceObject2 )

		MAP_ATTRIBUTE
		(
			"name",
			m_name,
			"",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE
		(
			"display",
			m_display,
			"If set, this transform hierarchy is displayed.\n"
			"Note that turning off display does not automatically turn\n"
			"off update.",
			Be::READWRITE | Be::PERSIST
		)

		MAP_ATTRIBUTE
		(
			"update",
			m_update,
			"If set, this transform hierarchy is updated every frame.",
			Be::READWRITE | Be::PERSIST
		)

		MAP_ATTRIBUTE
		( 
			"source",        
			m_source,
			"", 
			Be::READWRITE | Be::PERSIST
		)		
		MAP_ATTRIBUTE
		( 
			"dest",        
			m_dest,
			"", 
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE
		( 
			"sourceObject",        
			m_sourceObject,
			"Object to be rendered at the source", 
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE
		( 
			"destObject",        
			m_destObject,
			"Object to be rendered at the destination", 
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE
		( 
			"stretchObject",        
			m_stretchObject,
			"Object to be stretched from source to destination", 
			Be::READWRITE | Be::PERSIST
		)

		MAP_ATTRIBUTE
		(
			"moveObject",
			m_moveObject,
			"Object to be translated from source to destination",
			Be::READWRITE | Be::PERSIST
		)

		MAP_ATTRIBUTE
		( 
			"curveSets", 
			m_curveSets, 
			"Curvesets for animating things", 
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE
		(
			"length",
			m_length,
			"Distance between the source and the destination",
			Be::READ | Be::PERSIST
		)
		MAP_ATTRIBUTE
		(
			"lodLevel",
			m_lodLevel,
			"Current LOD level, 1(high) to 3(low)",
			Be::READ
		)

    EXPOSURE_END()
}
