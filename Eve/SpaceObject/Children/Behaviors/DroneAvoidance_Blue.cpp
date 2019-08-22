#include "StdAfx.h"
#include "DroneAvoidance.h"

BLUE_DEFINE( DroneAvoidance );

const Be::ClassInfo* DroneAvoidance::ExposeToBlue()
{
	EXPOSURE_BEGIN( DroneAvoidance, "" )
		MAP_INTERFACE( DroneAvoidance )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "behaviorWeight", m_behaviorWeight, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "visionRange", m_visionRange, "How far the drone looks for it's buddies", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "framesBetweenUpdates", m_framesBetweenUpdates, "updateFrequency to lessen the load", Be::READWRITE | Be::PERSIST )

		EXPOSURE_END()
}