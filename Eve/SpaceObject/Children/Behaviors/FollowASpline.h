#pragma once
#ifndef FollowASpline_H
#define FollowASpline_H
#include "Eve/SpaceObject/Children/EveChildBehaviorSystem.h"
#include "IBehavior.h"

struct FollowASplineData
{
	FollowASplineData() :
		tunnelLock( -1 ),
		tunnelPoint( 0 )
	{
	}

	int tunnelLock;
	int tunnelPoint;
};

BLUE_DECLARE( TriCurveSet );
BLUE_DECLARE( SplineTunnelGroup );
BLUE_DECLARE_VECTOR( SplineTunnelGroup );

BLUE_CLASS( FollowASpline ) :
	public IBehavior
{
public:
	EXPOSE_TO_BLUE();
	FollowASpline( IRoot* lockobj = nullptr );
	~FollowASpline();
	

	virtual size_t GetScratchMemorySize() const;
	virtual void InitializeScratch( const DroneAgent& drone, void* scratchMemory );
	virtual std::vector<Vector3> CalculateBehavior(std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
	                                               BehaviorGroup& group, EveChildBehaviorSystem& system, std::vector < std::vector<DroneAgent*>>& dronesInSearchRadius);
	void RenderDebugInfo(Tr2DebugRenderer& renderer, std::vector<DroneAgent>& agents, Matrix& parentWorldLocation);
	float GetBehaviorSearchRadius();
private:
	float ProcessTunnelEntrances(DroneAgent& agent, std::vector<SplineTunnel>& tunnels, FollowASplineData* data);
	void ProcessAssignedTunnel(DroneAgent& agent, std::vector<SplineTunnel>& tunnels, BehaviorGroup& group, FollowASplineData* data);

	PSplineTunnelGroupVector m_splineTunnels;
	TunnelGroupType m_tunnelGroupType;
	float m_behaviorWeight;	
	float m_smoothPullFactor;
	float m_cornerSmoothener;
	Vector3 m_desiredVector;
	std::vector <Vector3> m_targetPointVector;

	int m_framesBetweenUpdates;
	int m_frameCounter;
	std::vector<Vector3> m_lastPullForces;
};

TYPEDEF_BLUECLASS( FollowASpline );

#endif
