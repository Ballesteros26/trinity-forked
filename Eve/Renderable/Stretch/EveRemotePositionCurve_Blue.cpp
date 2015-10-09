////////////////////////////////////////////////////////////
//
//    Created:   October 2015
//    Copyright: CCP 2015
//

#include "StdAfx.h"
#include "EveRemotePositionCurve.h"

BLUE_DEFINE( EveRemotePositionCurve );

const Be::ClassInfo* EveRemotePositionCurve::ExposeToBlue()
{
	EXPOSURE_BEGIN( EveRemotePositionCurve, "" )
		MAP_INTERFACE( ITriVectorFunction )
		
	EXPOSURE_END()
}
