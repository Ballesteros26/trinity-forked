////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#pragma once

#include "ITr2ControllerAction.h"


BLUE_CLASS( Tr2ActionPlayMeshAnimation ) : public ITr2ControllerAction
{
public:
	enum PlayAction
	{
		PLAY,
		ENQUEUE_PLAY,
	};
	enum StopAction
	{
		STOP,
		ENQUEUE_STOP,
		NONE,
	};

	Tr2ActionPlayMeshAnimation( IRoot* lockobj = nullptr );

	EXPOSE_TO_BLUE();

	virtual void Start( Tr2Controller& controller );
	virtual void Stop( Tr2Controller& controller );
private:
	std::string m_animation;
	std::string m_mask;
	PlayAction m_playAction;
	StopAction m_stopAction;
	int32_t m_loops;
	float m_delay;
	float m_speed;
};

TYPEDEF_BLUECLASS( Tr2ActionPlayMeshAnimation );
