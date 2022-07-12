#pragma once
#ifndef SpawnDrones_H
#define SpawnDrones_H
#include "Eve/SpaceObject/Children/EveChildBehaviorSystem.h"
#include "IBehavior.h"

BLUE_CLASS( SpawnDrones ) :
	public IBehavior
{
public:
	EXPOSE_TO_BLUE();
	SpawnDrones( IRoot* lockobj = nullptr );
	~SpawnDrones();

	virtual std::vector<Vector3> CalculateBehavior( std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
		BehaviorGroup& group, EveChildBehaviorSystem& system, const std::vector<std::vector<DroneAgent*>>& dronesInSearchRadius );

private:
	bool m_enabled;
	bool m_addByCount;
	float m_seconds;
	float m_time;
	int m_count;
	Vector3 m_spawnPosition;
};

TYPEDEF_BLUECLASS( SpawnDrones );

#endif
