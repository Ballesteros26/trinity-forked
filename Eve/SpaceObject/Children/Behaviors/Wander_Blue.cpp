#include "StdAfx.h"
#include "Wander.h"


BLUE_DEFINE( Wander );

const Be::ClassInfo* Wander::ExposeToBlue()
{
	EXPOSURE_BEGIN( Wander, "" )
		MAP_INTERFACE( Wander )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "weightWander", m_weightWander, ":jessica-group: Wander", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "circleDistance", m_circleDistance, ":jessica-group: Wander", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "circleRadius", m_circleRadius, ":jessica-group: Wander", Be::READWRITE | Be::PERSIST )

	EXPOSURE_END()
}