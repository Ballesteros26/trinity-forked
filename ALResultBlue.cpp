#include "StdAfx.h"
#include "ALResultBlue.h"
#include "BlueExposure/include/BlueStdResult.h"

BLUE_DEFINE_EXCEPTION( ALError, BlueStdRuntimeError );
BLUE_DEFINE_EXCEPTION( ALDeviceLostError, ALError );
BLUE_DEFINE_EXCEPTION( ALDeviceNotAvailable, ALError );
BLUE_DEFINE_EXCEPTION( ALOutOfMemoryError, ALError );
BLUE_DEFINE_EXCEPTION( ALInvalidCallError, ALError );

namespace Be
{

// --------------------------------------------------------------------------------------
// Description:
//   Returns Python exception class for the given ALResult code.
// Arguments:
//   result - ALResult code
// Return value:
//   Python exception class appropriate for the given ALResult code
// --------------------------------------------------------------------------------------
BLUE_BEGIN_GET_EXCEPTION( ALResult )
	switch( result.GetCategory() )
	{
	case Result<HRESULT>::INVALID_CALL:
		return BLUE_GET_EXCEPTION( ALInvalidCallError );
	case Result<HRESULT>::DEVICE_LOST:
		return BLUE_GET_EXCEPTION( ALDeviceLostError );
	case Result<HRESULT>::DEVICE_NOT_AVAILABLE:
		return BLUE_GET_EXCEPTION( ALDeviceNotAvailable );
	case Result<HRESULT>::OUT_OF_MEMORY:
		return BLUE_GET_EXCEPTION( ALOutOfMemoryError );
	default:
		return BLUE_GET_EXCEPTION( ALError );
	}
BLUE_END_GET_EXCEPTION()

}
