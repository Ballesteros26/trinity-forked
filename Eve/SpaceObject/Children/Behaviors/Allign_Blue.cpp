#include "StdAfx.h"
#include "Allign.h"

BLUE_DEFINE( Allign );

const Be::ClassInfo* Allign::ExposeToBlue()
{
	EXPOSURE_BEGIN( Allign, "" )
		MAP_INTERFACE( Allign )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "behaviorWeight", m_behaviorWeight, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "visionRange", m_visionRange, "How far the drone looks for it's buddies", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "framesBetweenUpdates", m_framesBetweenUpdates, "updateFrequency to lessen the load", Be::READWRITE | Be::PERSIST )
		
		EXPOSURE_END()
}