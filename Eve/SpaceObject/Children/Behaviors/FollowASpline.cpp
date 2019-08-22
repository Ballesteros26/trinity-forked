#include "StdAfx.h"
#include "followASpline.h"

FollowASpline::FollowASpline( IRoot* lockobj ) :
	PARENTLOCK( m_splineTunnels ),
	m_behaviorWeight( 10.f ),
	m_smoothPullFactor( 0.8 ),
	m_cornerSmoothener( 0.8 ),
	m_tunnelGroupType( OTHER_TUNNELS ),
	m_frameCounter( 0 ),
	m_framesBetweenUpdates( 11 )
{
}

FollowASpline::~FollowASpline()
{
}

float FollowASpline::ProcessTunnelEntrances( DroneAgent& agent, std::vector<SplineTunnel>& tunnels, FollowASplineData* data )
{
	// not associated with a tunnel
	for ( auto tunnel = tunnels.begin(); tunnel != tunnels.end(); ++tunnel )
	{
		auto t = ( *tunnel );

		if( t.tunnelGroupID != OTHER_TUNNELS )
		{
			return 0;
		}

		Vector3 dist = t.splinePoints[ 0 ].pos - agent.position;
		float length = Length( dist );
		if ( length < t.pointOfNoReturnSize )
		{
			data->tunnelLock = t.tunnelID;
			data->tunnelPoint = 0;
		}
		else if ( length < t.pullSize )
		{
			if ( t.pullSize == t.pointOfNoReturnSize )
			{
				continue;
			}

			// normalize the distance between outer and inner spheres to increase pull-strength
			float mod = ( length - t.pointOfNoReturnSize ) / ( t.pullSize - t.pointOfNoReturnSize );
			mod = 1 - max( 0.f, min( mod, 1.f ) );
			m_desiredVector = dist;
			return min( 1.f, max( 0.f, 1 - m_smoothPullFactor + ( m_smoothPullFactor * mod ) ) );
		}
	}
	return 1.f;
}


void FollowASpline::ProcessAssignedTunnel( DroneAgent& agent, std::vector<SplineTunnel>& tunnels, BehaviorGroup& group, FollowASplineData* data )
{
	auto t = tunnels[ data->tunnelLock ];
	auto pID = data->tunnelPoint; // Point ID

	float lengthBetweenPoints;
	Vector3 targetVector = t.splinePoints[ pID ].pos - agent.position;
	Vector3 vectorBetween = Vector3( 0, 0, 0 );

	if ( t.splinePoints[ pID ] == ( *t.splinePoints.begin() ) )
	{
		vectorBetween = t.splinePoints[ pID ].rot;
	}
	else
	{
		vectorBetween = t.splinePoints[ pID - 1 ].rot;
	}


	lengthBetweenPoints = Length( vectorBetween );

	if( 0 != lengthBetweenPoints )
	{	// an offset is added to the target point so that they don't all follow the same line
		const float dotProd = Dot( targetVector, vectorBetween );
		const Vector3 vectorProj = ( dotProd / ( lengthBetweenPoints * lengthBetweenPoints ) ) * vectorBetween;
		const Vector3 offset = ( t.cylWidth / 2 ) * Normalize( vectorProj - targetVector );
		targetVector += offset;
	}

	m_targetPointVector.push_back( targetVector + agent.position );
	

	if ( t.splinePoints[ pID ] == ( *t.splinePoints.end() ) )
	{
		m_desiredVector = t.splinePoints[ pID ].rot;

		// the Dot product is positive if the agent is facing the target point
		if ( Dot( targetVector, Vector3( agent.rotation ) ) < 0 )
		{
			data->tunnelLock = -1;
			data->tunnelPoint = 0;
		}
	}
	else
	{
		float lengthFromShip = Length( targetVector );

		float blendingMod = 0;

		if ( lengthBetweenPoints != 0 )
		{
			blendingMod = min( 1.f, max( 0.f, ( lengthBetweenPoints - lengthFromShip ) / lengthBetweenPoints ) );
			blendingMod = blendingMod * blendingMod;
		}
		m_cornerSmoothener = min( 1.f, max( 0.f, m_cornerSmoothener ) );
		m_desiredVector = m_cornerSmoothener * ( 1 - blendingMod ) * Normalize( targetVector ) + 
							( 1 - m_cornerSmoothener ) * blendingMod * Normalize( t.splinePoints[ pID ].rot + targetVector );
		if ( Dot( Normalize( targetVector ), Normalize( m_desiredVector ) ) < m_cornerSmoothener )
		{
			m_desiredVector = targetVector;
		}

		if ((lengthFromShip - group.GetBoundingSphereRadius()) < (t.cylWidth)/1.5)
		{
			data->tunnelPoint++;
		}

		//rework into cylinder collision
		if ( lengthFromShip > ( group.GetBoundingSphereRadius() + lengthBetweenPoints * 1.5) && Dot( targetVector, vectorBetween ) < 0 )
		{
			data->tunnelLock = -1;
			data->tunnelPoint = 0;
		}
	}
}

size_t FollowASpline::GetScratchMemorySize() const
{
	return sizeof( FollowASplineData );
}

void FollowASpline::InitializeScratch( const DroneAgent& drone, void* scratchMemory )
{
	*static_cast<FollowASplineData*>( scratchMemory ) = FollowASplineData();
}

std::vector<Vector3> FollowASpline::CalculateBehavior( std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
														BehaviorGroup& group, EveChildBehaviorSystem& system, std::vector < std::vector<DroneAgent*>>& dronesInSearchRadius )
{
	std::vector<Vector3> forceVectors;
	if ( m_frameCounter == 0 )
	{
		m_lastPullForces.clear();
		auto tunnels = system.GetTunnels();
		m_targetPointVector.clear();
		
		// additional appending in case people want to add tunnels for specific BehaviorGroups
		if ( !m_splineTunnels.empty() )
		{
			int id = 0;
			if ( !tunnels.empty() )
			{
				id = tunnels.back().tunnelID;
			}

			for ( auto it = begin( m_splineTunnels ); it != end( m_splineTunnels ); ++it )
			{
				auto group = ( *it )->GetTunnels();

				for ( auto tunnel = begin( group ); tunnel != end( group ); ++tunnel )
				{
					( *tunnel ).tunnelID = id;
					id++;
					tunnels.push_back( *tunnel );
				}
			}
		}


		if ( tunnels.empty() )
		{
			return forceVectors;
		}

		auto data = static_cast< FollowASplineData* >( scratchData );
		for ( auto drone = agents.begin(); drone != agents.end(); ++drone, data++ )
		{
			m_desiredVector = Vector3( 0, 0, 0 );
			float rampingForce = 1;

			if ( data->tunnelLock == -1 )
			{
				rampingForce = ProcessTunnelEntrances( *drone, tunnels, data );
			}

			// tunnelLock can change in ProcessTunnelEntrances so if/else is not equivalent
			if ( data->tunnelLock != -1 )
			{
				ProcessAssignedTunnel( *drone, tunnels, group, data );
			}

			if ( m_desiredVector == Vector3( 0, 0, 0 ) )
			{
				m_lastPullForces.push_back( Vector3( 0, 0, 0 ) );
				continue;
			}

			Vector3 pullForce = Normalize( m_desiredVector );
			Vector3 forceOffset = pullForce * group.GetBoundingSphereRadius();
			forceVectors.push_back( drone->position + forceOffset );
			pullForce *= m_behaviorWeight * rampingForce;
			forceVectors.push_back( pullForce );
			drone->acceleration += pullForce;
			m_lastPullForces.push_back( pullForce );
		}
	}
	else
	{
		if ( m_lastPullForces.empty() )
		{
			return forceVectors;
		}
		int c = 0;
		for ( auto agent = agents.begin(); agent != agents.end(); ++agent, c++ )
		{
			agent->acceleration += m_lastPullForces[ c ];

			if ( group.m_collectForces )
			{
				Vector3 forceOffset = Normalize( m_lastPullForces[ c ] ) * group.GetBoundingSphereRadius();
				forceVectors.push_back( agent->position + forceOffset );
				forceVectors.push_back( m_lastPullForces[ c ] );
			}
		}
	}
	return forceVectors;
}

float FollowASpline::GetBehaviorSearchRadius()
{
	if ( m_frameCounter >= m_framesBetweenUpdates )
	{
		m_frameCounter = 0;
	}
	else
	{
		m_frameCounter++;
	}
	return -1;
}

void FollowASpline::RenderDebugInfo( Tr2DebugRenderer& renderer, std::vector<DroneAgent>& agents, Matrix& parentWorldLocation )
{
	if ( renderer.HasOption( this, "splineTunnels" ) )
	{
		for ( auto tPoint = m_targetPointVector.begin(); tPoint != m_targetPointVector.end(); ++tPoint )
		{
			renderer.DrawSphere( this, TranslationMatrix( ( *tPoint ) ) * parentWorldLocation,
				2, 6, Tr2DebugRenderer::Wireframe, 0xff114444 );
		}

		for ( auto t = m_splineTunnels.begin(); t != m_splineTunnels.end(); ++t )
		{
			( *t )->RenderDebugInfo( renderer, parentWorldLocation );
		}
		return;
	}
}