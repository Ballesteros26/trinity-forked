////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//
#include "StdAfx.h"
#include "EveSocketParameter.h"


BLUE_DEFINE_INTERFACE( IEveSocketParameter );
BLUE_DEFINE_ABSTRACT( EveSocketParameterBase );


const Be::ClassInfo* EveSocketParameterBase::ExposeToBlue()
{
	EXPOSURE_BEGIN( EveSocketParameterBase, "" )
	EXPOSURE_END()
}

#define SOCKET_PARAM_EXPOSE_TO_BLUE( _className )\
	BLUE_DEFINE( ##_className## );\
	const Be::ClassInfo* ##_className##::ExposeToBlue()\
	{\
		EXPOSURE_BEGIN( ##_className##, "" )\
			MAP_INTERFACE( IEveSocketParameter )\
			MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )\
			MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE | Be::PERSIST )\
			EXPOSURE_END()\
	}


SOCKET_PARAM_EXPOSE_TO_BLUE( EveSocketParameterFloat );
SOCKET_PARAM_EXPOSE_TO_BLUE( EveSocketParameterVector2 );
SOCKET_PARAM_EXPOSE_TO_BLUE( EveSocketParameterVector3 );
SOCKET_PARAM_EXPOSE_TO_BLUE( EveSocketParameterVector4 );
SOCKET_PARAM_EXPOSE_TO_BLUE( EveSocketParameterColor );
