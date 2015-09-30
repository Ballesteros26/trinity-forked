#pragma once
#ifndef Tr2GpuTimerALContext_H
#define Tr2GpuTimerALContext_H


class Tr2GpuTimerALContext
{
public:
	Tr2GpuTimerALContext( Tr2GpuTimerAL& timer, Tr2RenderContextAL& renderContext )
		:m_timer( timer ),
		m_renderContext( renderContext )
	{
		m_timer.Begin( m_renderContext );
	}
	~Tr2GpuTimerALContext()
	{
		m_timer.End( m_renderContext );
	}
private:
	Tr2GpuTimerAL& m_timer;
	Tr2RenderContextAL& m_renderContext;
};


#endif