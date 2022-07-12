#include "StdAfx.h"
#include "SpawnDrones.h"

BLUE_DEFINE( SpawnDrones );

const Be::ClassInfo* SpawnDrones::ExposeToBlue()
{
	EXPOSURE_BEGIN( SpawnDrones, "" )
		MAP_INTERFACE( SpawnDrones )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE( "enabled", m_enabled, "Should this behavior be active", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "add by count", m_addByCount, "If enabled behavior will spawn count of drones instead of count drones every x seconds", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "spawnPosition", m_spawnPosition, "Where should the drones spawn", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "seconds", m_seconds, "How many seconds until the next one spawns, set to -1 if you don't want any drones added", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "count", m_count, "How many drones should spawn", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "time", m_time, "Time left until next agent spawns", Be::READ | Be::PERSIST )


	EXPOSURE_END()
}