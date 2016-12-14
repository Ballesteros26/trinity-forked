////////////////////////////////////////////////////////////
//
//    Created:   February 2013
//    Copyright: CCP 2013
//

#include "StdAfx.h"
#include "Tr2GpuBufferAlias.h"

BLUE_DEFINE( Tr2GpuBufferAlias );

const Be::ClassInfo* Tr2GpuBufferAlias::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2GpuBufferAlias, "" )

		MAP_INTERFACE( INotify )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITr2GpuBuffer )
		MAP_INTERFACE( Tr2GpuBufferAlias )

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS(
			"__init__",
			py__init__,
			3,
			"Arguments (optional):\n"
			":param source: source GPU buffer\n"
			":param idx: buffer index in the source\n"
			":param format: buffer pixel format (trinity.PIXEL_FORMAT.foo)\n" )
		MAP_ATTRIBUTE( 
			"source", 
			m_source, 
			"Source GPU buffer", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( 
			"index", 
			m_index, 
			"Index of of GPU buffer in the source (if the source supports more than one buffer)", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE_WITH_CHOOSER(
			"format",
			m_format,
			"Buffer pixel format (trinity.PIXEL_FORMAT.foo)",
			Be::READWRITE | Be::ENUM | Be::PERSIST | Be::NOTIFY, 
			Tr2RenderContextEnum_PixelFormat_Chooser )
		MAP_PROPERTY_READONLY( 
			"isValid",
			IsValid,
			"Is the underlying GPU object valid" )
		MAP_PROPERTY_READONLY( 
			"count",
			GetCount,
			"Number of elements in the buffer" )
	EXPOSURE_END()
}
