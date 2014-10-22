#include "StdAfx.h"
#include "Tr2GpuStructuredBuffer.h"

BLUE_DEFINE( Tr2GpuStructuredBuffer );

const Be::ClassInfo* Tr2GpuStructuredBuffer::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2GpuStructuredBuffer, "" )

		MAP_INTERFACE( INotify )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITr2GpuBuffer )
		MAP_INTERFACE( Tr2GpuStructuredBuffer )

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS(
			"__init__",
			py__init__,
			3,
			"Arguments (optional):\n"
			"count - number of elements in the buffer\n"
			"stride - size of buffer element in bytes\n"
			"cpuWritable - if CPU should have write access to the buffer\n" )
		MAP_ATTRIBUTE( 
			"count", 
			m_count, 
			"Number of elements in the buffer", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( 
			"stride", 
			m_stride, 
			"Size of buffer element in bytes", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( 
			"creationFlags", 
			m_creationFlags, 
			"If CPU should have write access to the buffer", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_PROPERTY_READONLY( 
			"isValid",
			IsValid,
			"Is the underlying GPU object valid" )
	EXPOSURE_END()
}
