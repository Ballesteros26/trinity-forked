////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlayMeshAnimation.h"
#include "Controllers/Tr2Controller.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Tr2GrannyAnimation.h"


Tr2ActionPlayMeshAnimation::Tr2ActionPlayMeshAnimation( IRoot* lockobj )
	:m_loops( -1 ),
	m_playAction( ENQUEUE_PLAY ),
	m_stopAction( ENQUEUE_STOP ),
	m_delay( 0 ),
	m_speed( 1 )
{
}

void Tr2ActionPlayMeshAnimation::Start( Tr2Controller& controller )
{
	if( m_animation.empty() )
	{
		return;
	}
	EveSpaceObject2Ptr owner = BlueCastPtr( controller.GetOwner() );
	if( !owner )
	{
		return;
	}
	auto ac = owner->GetAnimationController();
	if( !ac )
	{
		return;
	}
	if( !m_mask.empty() )
	{
		ac->AddAnimationLayerWithTrackMask( m_mask.c_str(), m_mask.c_str() );
	}
	ac->PlayLayerAnimationByName( m_mask.empty() ? nullptr : m_mask.c_str(), m_animation.c_str(), m_playAction == PLAY, std::max( m_loops, 0 ), m_delay, m_speed, false );
}

void Tr2ActionPlayMeshAnimation::Stop( Tr2Controller& controller )
{
	if( m_animation.empty() )
	{
		return;
	}
	EveSpaceObject2Ptr owner = BlueCastPtr( controller.GetOwner() );
	if( !owner )
	{
		return;
	}
	auto ac = owner->GetAnimationController();
	if( !ac )
	{
		return;
	}
	auto layer = ac->GetAnimationLayer( m_mask.empty() ? nullptr : m_mask.c_str() );
	if( !layer )
	{
		return;
	}
	switch( m_stopAction )
	{
	case STOP:
		layer->ClearAnimations();
		break;
	case ENQUEUE_STOP:
		layer->EndAnimation();
		break;
	default:
		break;
	}
}
