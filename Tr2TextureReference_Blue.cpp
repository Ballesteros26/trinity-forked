#include "StdAfx.h"
#include "Tr2TextureReference.h"

BLUE_DEFINE( Tr2TextureReference );


const Be::ClassInfo* Tr2TextureReference::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2TextureReference, "" )
        MAP_INTERFACE( Tr2TextureReference )
        MAP_INTERFACE( ITr2TextureProvider )
    EXPOSURE_END()
}
