////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionAnimateValue.h"
#include "Controllers/Tr2Controller.h"


namespace
{
	Tr2ActionAnimateValue* s_action = nullptr;
	float s_stateTime = 0;

	float StateTime()
	{
		return s_stateTime;
	}

	float Curve( float time )
	{
		return s_action->GetCurveValue( s_stateTime );
	}
}


Tr2ActionAnimateValue::Tr2ActionAnimateValue( IRoot* )
	:m_controller( nullptr ),
	m_value( "Curve(StateTime())" ),
	m_startTime( 0 )
{
}

void Tr2ActionAnimateValue::Link( Tr2Controller& controller )
{
	m_controller = &controller;
	std::unordered_map<std::string, IRoot*> roots;
	controller.GetBindingPathRoots( roots );
	m_destination.Link( roots );
	m_evaluator.SetExpr( m_value.c_str(), controller );
}

void Tr2ActionAnimateValue::Unlink()
{
	m_destination.Unlink();
	m_controller = nullptr;
	m_evaluator.Clear();
}

void Tr2ActionAnimateValue::Start( Tr2Controller& controller )
{
	if( !m_evaluator.IsExpressionValid() || !m_destination.IsValid() )
	{
		return;
	}
	m_startTime = BeOS->GetCurrentFrameTime();
	controller.RegisterUpdateable( *this );
}

void Tr2ActionAnimateValue::Stop( Tr2Controller& controller )
{
	if( !m_curve )
	{
		return;
	}
	controller.UnRegisterUpdateable( *this );
}

void Tr2ActionAnimateValue::Update( Be::Time realTime, Be::Time simTime )
{
	if( !m_evaluator.IsExpressionValid() || !m_destination.IsValid() )
	{
		return;
	}
	s_stateTime = TimeAsFloat( simTime - m_startTime );
	s_action = this;
	auto value = m_evaluator.Eval();
	s_stateTime = 0;
	s_action = nullptr;
	if( value.first )
	{
		m_destination.SetValue( value.second );
	}
}

bool Tr2ActionAnimateValue::OnModified( Be::Var* value )
{
	if( !m_controller )
	{
		return true;
	}
	if( IsMatch( value, m_destination.m_path ) || IsMatch( value, m_destination.m_attribute ) )
	{
		std::unordered_map<std::string, IRoot*> roots;
		m_controller->GetBindingPathRoots( roots );
		m_destination.Link( roots );
	}
	else if( IsMatch( value, m_value ) )
	{
		m_evaluator.SetExpr( m_value.c_str(), *m_controller );
	}
	return true;
}

bool Tr2ActionAnimateValue::IsBindingValid() const
{
	return m_destination.IsValid();
}

float Tr2ActionAnimateValue::GetCurveValue( float time ) const
{
	if( !m_curve )
	{
		return 0;
	}
	return m_curve->GetValueAt( time );
}