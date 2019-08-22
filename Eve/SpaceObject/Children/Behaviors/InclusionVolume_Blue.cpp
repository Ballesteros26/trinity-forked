#include "StdAfx.h"
#include "InclusionVolume.h"

BLUE_DEFINE( InclusionVolume );

const Be::ClassInfo* InclusionVolume::ExposeToBlue()
{
	EXPOSURE_BEGIN( InclusionVolume, "" )
		MAP_INTERFACE( InclusionVolume )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "inclusionVolumes", m_inclusionVolumes, "", Be::READ | Be::PERSIST )
		MAP_ATTRIBUTE( "behaviorWeight", m_behaviorWeight, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "framesBetweenUpdates", m_framesBetweenUpdates, "updateFrequency to lessen the load", Be::READWRITE | Be::PERSIST )

		EXPOSURE_END()
}