#include "StdAfx.h"
#include "Tr2ShaderBuffer.h"

BLUE_DEFINE( Tr2ShaderBuffer );

const Be::ClassInfo* Tr2ShaderBuffer::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2ShaderBuffer, "" )
        MAP_INTERFACE( Tr2ShaderBuffer )

		MAP_ATTRIBUTE
		(
			"size",
			m_size,
			"",
			Be::READ
		)

		MAP_METHOD_AND_WRAP(
			"SetData",
			SetDataFromScript,
			""
		)

    EXPOSURE_END()
}