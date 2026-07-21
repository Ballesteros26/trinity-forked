// Copyright © 2025 CCP ehf.
//
//    Description:
//      An interface for audio functionality related to stretch effects.

#pragma once

#include "ITr2AudEmitter.h"

BLUE_INTERFACE( IStretchAudio ) : public IRoot
{
    virtual void Start() = 0;
    virtual void Stop() = 0;
	virtual void Update( Vector3& sourcePosition, Vector3& destPosition) = 0;
	virtual ITr2AudEmitterPtr FindEmitterByName( const char* name ) = 0;
};
