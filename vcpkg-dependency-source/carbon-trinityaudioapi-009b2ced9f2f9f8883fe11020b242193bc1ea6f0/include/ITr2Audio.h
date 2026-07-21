// Copyright © 2025 CCP ehf.
//
//    Description:
//      An interface allowing for an abstraction of audio on an asset.
//      Note: This interface is deprecated. Please use IStretchAudio instead.

#pragma once

#include "ITr2AudEmitter.h"

BLUE_INTERFACE( ITr2Audio ) : public IRoot
{
	virtual void Update( Vector3& sourcePosition, Vector3& destPosition) = 0;
	virtual ITr2AudEmitterPtr FindEmitterByName( const char* name ) = 0;
};
