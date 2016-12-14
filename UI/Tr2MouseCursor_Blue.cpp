////////////////////////////////////////////////////////////
//
//    Created:   June 2012
//    Copyright: CCP 2012
//

#include "StdAfx.h"
#include "Tr2MouseCursor.h"
#include "Tr2HostBitmap.h"

BLUE_DEFINE( Tr2MouseCursor );

const Be::ClassInfo* Tr2MouseCursor::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2MouseCursor, "" )

		MAP_INTERFACE( Tr2MouseCursor )

		MAP_METHOD_AND_WRAP( 
			"__init__", 
			py__init__, 
			"Creates a cursor from bitmap\n"
			":param bitmap: bitmap containing cursor image\n"
			":param hotspotX: horizontal offset to cursor hotspot\n"
			":param hotspotY: vertical offset to cursor hotspot\n"
			)

		MAP_METHOD_AND_WRAP( 
			"Create", 
			Create, 
			"Creates a new mouse cursor image\n" 
			":param bitmap: bitmap containing cursor image\n"
			":param hotspotX: horizontal offset to cursor hotspot\n"
			":param hotspotY: vertical offset to cursor hotspot\n"
			)

		MAP_METHOD_AND_WRAP( 
			"IsValid",
			IsValid,
			"Check if the cursor is valid (has image)"
			)

		MAP_PROPERTY_READONLY( 
			"isValid",
			IsValid,
			"Is the cursor valid (has image)"
			)

	EXPOSURE_END()
}