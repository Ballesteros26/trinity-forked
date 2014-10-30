#include "StdAfx.h"
#include "EveDistanceField.h"

BLUE_DEFINE( EveDistanceField );

const Be::ClassInfo* EveDistanceField::ExposeToBlue()
{
	EXPOSURE_BEGIN( EveDistanceField, "" )
        MAP_INTERFACE( EveDistanceField )

		MAP_ATTRIBUTE( "camera", m_camera, "na", Be::READWRITE )
		MAP_ATTRIBUTE( "objects", m_objects, "na", Be::READWRITE )
		MAP_ATTRIBUTE( "curveSet", m_curveSet, "na", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "distance", m_distance, "na", Be::READWRITE )
		MAP_ATTRIBUTE( "timeAdjustmentSecondsOut", 
						m_timeAdjustmentSecondsOut, 
						"Adjust how long it takes to settle on a new value when zooming out", 
						Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "timeAdjustmentSecondsIn", 
						m_timeAdjustmentSecondsIn, 
						"Adjust how long it takes to settle on a new value when zooming in", 
						Be::READWRITE | Be::PERSIST )

	EXPOSURE_END()
}