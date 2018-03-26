////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2StateMachineTransition.h"
#include "Tr2StateMachineState.h"


BLUE_DEFINE( Tr2StateMachineTransition );

const Be::ClassInfo* Tr2StateMachineTransition::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2StateMachineTransition, "" )
		MAP_INTERFACE( Tr2StateMachineTransition )
		MAP_INTERFACE( INotify )

		MAP_ATTRIBUTE( "name", m_destinationName, "Destination state name", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "condition", m_condition, "Condition expression", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_PROPERTY_READONLY( "isConditionValid", IsConditionValid, "" )

		MAP_METHOD_AND_WRAP( "GetState", GetSource, "" )
	EXPOSURE_END()
}
