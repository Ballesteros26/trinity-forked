#include "StdAfx.h"
#include "ApproachGroup.h"

BLUE_DEFINE( ApproachGroup );

const Be::ClassInfo* ApproachGroup::ExposeToBlue()
{
	EXPOSURE_BEGIN( ApproachGroup, "" )
		MAP_INTERFACE( ApproachGroup )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "behaviorWeight", m_behaviorWeight, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "visionRange", m_visionRange, "How far the drone looks for it's buddies", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "framesBetweenUpdates", m_framesBetweenUpdates, "updateFrequency to lessen the load", Be::READWRITE | Be::PERSIST )

		EXPOSURE_END()
}