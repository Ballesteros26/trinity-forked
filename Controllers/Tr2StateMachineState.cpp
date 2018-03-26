////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2StateMachineState.h"
#include "Tr2StateMachine.h"
#include "Tr2StateMachineTransition.h"
#include "Actions/ITr2ControllerAction.h"
#include "Finalizers/ITr2StateMachineStateFinalizer.h"


Tr2StateMachineState::Tr2StateMachineState( IRoot* lockobj )
	:PARENTLOCK( m_actions ),
	PARENTLOCK( m_transitions ),
	m_stateMachine( nullptr ),
	m_isActive( false ),
	m_isFinalizing( false )
{
	m_actions.SetNotify( this );
	m_transitions.SetNotify( this );
}

bool Tr2StateMachineState::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_finalizer ) )
	{
		if( m_finalizer && m_stateMachine )
		{
			m_finalizer->Link( *m_stateMachine->GetController() );
		}
	}
	return true;
}

void Tr2StateMachineState::OnListModified( long event, ssize_t key, ssize_t key2, IRoot* value, const IList* list )
{
	if( list == &m_actions )
	{
		switch( event & BELIST_EVENTMASK )
		{
		case BELIST_INSERTED:
			if( m_stateMachine )
			{
				if( ITr2ControllerActionPtr action = BlueCastPtr( value ) )
				{
					action->Link( *m_stateMachine->GetController() );
					if( m_isActive )
					{
						action->Start( *m_stateMachine->GetController() );
					}
				}
			}
			break;
		case BELIST_REMOVED:
			if( ITr2ControllerActionPtr action = BlueCastPtr( value ) )
			{
				if( m_stateMachine )
				{
					if( m_isActive )
					{
						action->Stop( *m_stateMachine->GetController() );
					}
				}
				action->Unlink();
			}
			break;
		default:
			break;
		}
	}
	else if( list == &m_transitions )
	{
		switch( event & BELIST_EVENTMASK )
		{
		case BELIST_INSERTED:
			if( m_stateMachine )
			{
				if( Tr2StateMachineTransitionPtr transition = BlueCastPtr( value ) )
				{
					transition->Link( *this );
				}
			}
			break;
		case BELIST_REMOVED:
			if( Tr2StateMachineTransitionPtr transition = BlueCastPtr( value ) )
			{
				transition->Unlink();
			}
			break;
		default:
			break;
		}
	}
}

void Tr2StateMachineState::Link( const Tr2StateMachine& stateMachine )
{
	Unlink();

	m_stateMachine = &stateMachine;
	for( auto it = begin( m_transitions ); it != end( m_transitions ); ++it )
	{
		( *it )->Link( *this );
	}
	for( auto it = begin( m_actions ); it != end( m_actions ); ++it )
	{
		( *it )->Link( *stateMachine.GetController() );
	}
	if( m_finalizer )
	{
		m_finalizer->Link( *stateMachine.GetController() );
	}
}

void Tr2StateMachineState::Unlink()
{
	if( !m_stateMachine )
	{
		return;
	}
	Stop();
	m_stateMachine = nullptr;
	for( auto it = begin( m_transitions ); it != end( m_transitions ); ++it )
	{
		( *it )->Unlink();
	}
	for( auto it = begin( m_actions ); it != end( m_actions ); ++it )
	{
		( *it )->Unlink();
	}
	if( m_finalizer )
	{
		m_finalizer->Unlink();
	}
}

Tr2StateMachineState* Tr2StateMachineState::Update()
{
	if( !m_isActive )
	{
		return nullptr;
	}
	if( m_isFinalizing )
	{
		auto next = GetNextState();
		if( !next )
		{
			m_isActive = false;
			Start();
		}
		if( !m_finalizer || m_finalizer->CanTransition( *m_stateMachine->GetController() ) )
		{
			return next;
		}
		return nullptr;
	}
	for( auto it = begin( m_transitions ); it != end( m_transitions ); ++it )
	{
		if( ( *it )->CanActivate() && ( *it )->GetDestination() )
		{
			Stop();
			if( m_isFinalizing )
			{
				return nullptr;
			}
			return ( *it )->GetDestination();
		}
	}
	return nullptr;
}

Tr2StateMachineState* Tr2StateMachineState::GetNextState() const
{
	for( auto it = begin( m_transitions ); it != end( m_transitions ); ++it )
	{
		if( ( *it )->CanActivate() && ( *it )->GetDestination() )
		{
			return ( *it )->GetDestination();
		}
	}
	return nullptr;
}

const Tr2StateMachine* Tr2StateMachineState::GetStateMachine() const
{
	return m_stateMachine;
}

const std::string& Tr2StateMachineState::GetName() const
{
	return m_name;
}

void Tr2StateMachineState::Start()
{
	if( m_isActive )
	{
		return;
	}
	if( m_stateMachine )
	{
		for( auto it = begin( m_actions ); it != end( m_actions ); ++it )
		{
			( *it )->Start( *m_stateMachine->GetController() );
		}
		m_isActive = true;
		m_isFinalizing = false;
	}
}

void Tr2StateMachineState::Stop()
{
	if( !m_isActive || m_isFinalizing )
	{
		return;
	}
	if( m_stateMachine )
	{
		for( auto it = begin( m_actions ); it != end( m_actions ); ++it )
		{
			( *it )->Stop( *m_stateMachine->GetController() );
		}
		if( m_finalizer )
		{
			if( !m_finalizer->CanTransition( *m_stateMachine->GetController() ) )
			{
				m_isFinalizing = true;
				return;
			}
		}
	}
	m_isActive = false;
}

IRoot* Tr2StateMachineState::GetStateMachinePtr() const
{
	return m_stateMachine ? m_stateMachine->GetRawRoot() : nullptr;
}
