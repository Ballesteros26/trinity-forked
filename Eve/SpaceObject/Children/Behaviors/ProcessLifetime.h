#pragma once
#ifndef FollowASpline_H
#define FollowASpline_H
#include "Eve/SpaceObject/Children/EveChildBehaviorSystem.h"
#include "IBehavior.h"

BLUE_DECLARE( TriCurveSet );

BLUE_CLASS( ProcessLifetime ) :
	public IBehavior
{
public:
	EXPOSE_TO_BLUE();
	ProcessLifetime( IRoot* lockobj = nullptr );
	~ProcessLifetime();
	
	virtual std::vector<Vector3> CalculateBehavior(std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
	                                               BehaviorGroup& sys, EveChildBehaviorSystem& system, std::vector<std::vector<DroneAgent*>>& dronesInSearchRadius);
	void RenderDebugInfo(Tr2DebugRenderer& renderer, std::vector<DroneAgent>& agents, Matrix& parentWorldLocation);
	float GetBehaviorSearchRadius();
private:
	float m_firstAgentLifetime; // debug visualization 
	TunnelGroupType m_tunnelGroupType;

	float m_returningAge; // how old is the drone when it starts returning (-1 means it'll last forever)
	bool m_useEntryTunnels;
	bool m_useExitTunnels;
};

TYPEDEF_BLUECLASS( ProcessLifetime );

#endif
