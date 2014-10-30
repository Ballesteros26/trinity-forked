#pragma once
#ifndef EveUpdateContext_h
#define EveUpdateContext_h
#include "Tr2GPUParticlePoolManager.h"

class EveUpdateContext
{
public:
	EveUpdateContext() {}
	EveUpdateContext( Be::Time time ) : 
		m_lastTime( 0 ),
		m_currentTime( 0 )
	{
		SetTime( time );
	}

	// Base attribute time
	const Be::Time GetTime() const
	{
		return m_currentTime;
	}
	void SetTime( Be::Time time )
	{
		m_lastTime = m_currentTime;
		m_currentTime = time;
	}

	// get deltaT to last
	float GetDeltaT() const
	{
		// get usefull time info
		float deltaT = 0.f;
		if( m_lastTime != 0 )
			deltaT = TimeAsFloat( m_currentTime - m_lastTime );
		return deltaT;
	}

	// Any extra objects you would like to pass along
	Tr2GPUParticlePoolManager* GetParticlePoolManager()
	{
		return m_gpuParticleManager;
	}
	void SetParticlePoolManager( Tr2GPUParticlePoolManager* manager )
	{
		m_gpuParticleManager = manager;
	}

private:
	Be::Time m_currentTime;
	Be::Time m_lastTime;

	// extra stuff
	Tr2GPUParticlePoolManagerPtr m_gpuParticleManager;		
};

#endif //EveUpdateContext_h
