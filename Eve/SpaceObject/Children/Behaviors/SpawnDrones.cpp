#include "StdAfx.h"
#include "SpawnDrones.h"
#include "ProcessLifetime.h"

SpawnDrones::SpawnDrones( IRoot* lockobj ) :
	m_enabled( true ),
	m_addByCount( false ),
	m_seconds( -1 ),
	m_time( 0.f ),
	m_count( 1 ),
	m_spawnPosition( 0, 0, 0 )
{
}

SpawnDrones::~SpawnDrones()
{
}

std::vector<Vector3> SpawnDrones::CalculateBehavior( std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
	BehaviorGroup& group, EveChildBehaviorSystem& system, const std::vector<std::vector<DroneAgent*>>& dronesInSearchRadius )
{
	std::vector<Vector3> noNeedToReturnForces;
	if( !m_enabled )
	{
		return noNeedToReturnForces;
	}

	// If m_addByCount is toggled on the behavior adds agents by count
	if( m_addByCount == true )
	{
		for( unsigned int i = 0; i < m_count; i++ )
		{
			group.AddAgent();
		}
		
		m_addByCount = false;
	}

	if( m_seconds <= 0.0 )
	{
		return noNeedToReturnForces;
	}

	m_time += deltaTime;

	if( m_time > m_seconds && m_seconds >= 0.0 )
	{
		auto behavior = group.GetBehaviorByName( "ProcessLifetime" );
		if( behavior != nullptr )
		{
			auto processLifetime = dynamic_cast<ProcessLifetime*> ( behavior );
			if( processLifetime )
			{
				std::vector<Vector3> spawnPoints = processLifetime->GetEntrancePoints();
				if( !spawnPoints.empty() )
				{
					const auto randomNbr = rand() % spawnPoints.size();
					m_spawnPosition = spawnPoints.at( randomNbr );
				}
			}
		}

		group.m_spawnPosition = m_spawnPosition;

		for( unsigned int i = 0; i < m_count; i++ )
		{
			group.AddAgent();
		}
		m_time = 0.0f;
	}




	return noNeedToReturnForces;
}
