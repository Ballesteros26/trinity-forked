////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlayMeshAnimation.h"


BLUE_DEFINE( Tr2ActionPlayMeshAnimation );

namespace
{
	Be::VarChooser PlayActionChooser[] =
	{
		{ "Play", BeCast( Tr2ActionPlayMeshAnimation::PLAY ), "Play animation immediately" },
		{ "Enqueue Play", BeCast( Tr2ActionPlayMeshAnimation::ENQUEUE_PLAY ), "Play animation after all other animations complete" },
		{ 0 }
	};
	Be::VarChooser StopActionChooser[] =
	{
		{ "Stop", BeCast( Tr2ActionPlayMeshAnimation::STOP ), "Stop animation immediately" },
		{ "Enqueue Stop", BeCast( Tr2ActionPlayMeshAnimation::ENQUEUE_STOP ), "Stop animation after loop finishes" },
		{ "None", BeCast( Tr2ActionPlayMeshAnimation::NONE ), "Do not stop animation" },
		{ 0 }
	};

}

const Be::ClassInfo* Tr2ActionPlayMeshAnimation::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ActionPlayMeshAnimation, "" )
		MAP_INTERFACE( Tr2ActionPlayMeshAnimation )
		MAP_INTERFACE( ITr2ControllerAction )

		MAP_ATTRIBUTE( "animation", m_animation, "Animation name", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "mask", m_mask, "Animation mask/layer name", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE_WITH_CHOOSER( "playAction", m_playAction, "", Be::READWRITE | Be::PERSIST | Be::ENUM, PlayActionChooser )
		MAP_ATTRIBUTE_WITH_CHOOSER( "stopAction", m_stopAction, "", Be::READWRITE | Be::PERSIST | Be::ENUM, StopActionChooser )
		MAP_ATTRIBUTE( "loops", m_loops, "Number of loops animation is supposed to play (<=0 for infinite)", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "delay", m_delay, "Delay animation start in seconds", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "speed", m_speed, "Animation speed factor", Be::READWRITE | Be::PERSIST )
	EXPOSURE_END()
}
