////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2StateMachineTransition.h"
#include "Tr2StateMachineState.h"
#include "Tr2StateMachine.h"


Tr2StateMachineTransition::Tr2StateMachineTransition( IRoot* lockobj )
	:m_source( nullptr )
{
}

bool Tr2StateMachineTransition::OnModified( Be::Var* value )
{
	if( !m_source )
	{
		return true;
	}

	if( IsMatch( value, m_condition ) )
	{
		m_evaluator.SetExpr( m_condition.c_str(), *m_source->GetStateMachine() );
	}
	else if( IsMatch( value, m_destinationName ) )
	{
		UpdateDestination();
	}
	return true;
}

void Tr2StateMachineTransition::Link( const Tr2StateMachineState& state )
{
	Unlink();

	m_source = &state;
	if( m_source->GetStateMachine() )
	{
		UpdateDestination();
	}
	m_evaluator.SetExpr( m_condition.c_str(), *m_source->GetStateMachine() );
}

void Tr2StateMachineTransition::Unlink()
{
	m_source = nullptr;
	m_destination = nullptr;
	m_evaluator.Clear();
}

Tr2StateMachineState* Tr2StateMachineTransition::GetDestination() const
{
	return m_destination;
}

void Tr2StateMachineTransition::UpdateDestination()
{
	m_destination = m_source->GetStateMachine()->GetStateByName( m_destinationName.c_str() );
	if( !m_destination )
	{
		CCP_LOGERR( "Invalid destination state name %s for state machine transition", m_destinationName.c_str() );
	}
}

bool Tr2StateMachineTransition::CanActivate() const
{
	if( !m_source )
	{
		return false;
	}
	auto result = m_evaluator.Eval();
	if( !result.first )
	{
		return false;
	}
	return result.second != 0;
}

IRoot* Tr2StateMachineTransition::GetSource() const
{
	return m_source->GetRawRoot();
}

bool Tr2StateMachineTransition::IsConditionValid() const
{
	return m_evaluator.IsExpressionValid();
}
