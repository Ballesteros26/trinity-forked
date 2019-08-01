////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionAnimateValue.h"
#include "Controllers/Tr2Controller.h"
#include "Tr2ExpressionTermInfo.h"
#include "Controllers/Tr2ControllerFloatVariable.h"


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
		if( !s_action )
		{
			return 0;
		}
		return s_action->GetCurveValue( time );
	}

	void ModifyParser( mu::Parser& parser )
	{
		parser.DefineFun( "StateTime", StateTime );
		parser.DefineFun( "Curve", Curve );
	}
}


Tr2ActionAnimateValue::Tr2ActionAnimateValue( IRoot* )
	:m_controller( nullptr ),
	m_value( "Curve(StateTime())" ),
	m_startTime( 0 ),
	m_lastSimTime( 0 ),
	m_delayBinding( false )
{
}

void Tr2ActionAnimateValue::Link( Tr2Controller& controller )
{
	m_controller = &controller;
	if( !HasDelayedBinding() )
	{
		LinkDestination( controller );
	}
	m_evaluator.SetExpr( m_value.c_str(), controller, ModifyParser );
}

void Tr2ActionAnimateValue::Unlink()
{
	m_destination.Unlink();
	m_controller = nullptr;
	m_evaluator.Clear();
}

void Tr2ActionAnimateValue::Start( Tr2Controller& controller )
{
	if( HasDelayedBinding() )
	{
		LinkDestination( controller );
	}
	if( !m_destination.IsValid() )
	{
		return;
	}
	m_startTime = BeOS->GetCurrentFrameTime();
	controller.RegisterUpdateable( *this );
}

void Tr2ActionAnimateValue::Stop( Tr2Controller& controller )
{
	controller.UnRegisterUpdateable( *this );
}

void Tr2ActionAnimateValue::Update( Be::Time realTime, Be::Time simTime )
{
	m_lastSimTime = simTime;
	if( !m_destination.IsValid() )
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
	if( IsMatch( value, m_destination.m_path ) || IsMatch( value, m_destination.m_attribute ) || IsMatch( value, m_destination.m_object ) || IsMatch( value, m_delayBinding ) )
	{
		if( !HasDelayedBinding() )
		{
			LinkDestination( *m_controller );
		}
	}
	else if( IsMatch( value, m_value ) )
	{
		m_evaluator.SetExpr( m_value.c_str(), *m_controller, ModifyParser );
	}
	return true;
}

bool Tr2ActionAnimateValue::IsBindingValid() const
{
	return m_destination.IsValid();
}

bool Tr2ActionAnimateValue::IsExpressionValid() const
{
	return m_evaluator.IsExpressionValid();
}

float Tr2ActionAnimateValue::GetCurveValue( float time ) const
{
	if( !m_curve )
	{
		return 0;
	}
	return m_curve->GetValueAt( time );
}

IRootPtr Tr2ActionAnimateValue::GetDestination() const
{
	return m_destination.GetBoundObject();
}

bool Tr2ActionAnimateValue::IsAttrExpressionValid( const char* ) const
{
	return IsExpressionValid();
}

std::vector<Tr2ExpressionTermInfoPtr> Tr2ActionAnimateValue::GetExpressionTermInfo() const
{
	std::vector<Tr2ExpressionTermInfoPtr> result;
	m_evaluator.GetExpressionTermInfo( result );
	result.push_back( Tr2ExpressionTermInfo::Function( "Controller", "Curve", "x", "action curve value at time x" ) );

	if( m_controller )
	{
		auto& variables = m_controller->GetVariables();
		for( auto it = begin( variables ); it != end( variables ); ++it )
		{
			result.push_back( Tr2ExpressionTermInfo::Variable( "Variables", ( *it )->GetName().c_str(), "controller variable" ) );
		}
	}
	return result;
}

void Tr2ActionAnimateValue::LinkDestination( const Tr2Controller& controller )
{
	std::unordered_map<std::string, IRoot*> roots;
	controller.GetBindingPathRoots( roots );
	m_destination.Link( roots );
}

bool Tr2ActionAnimateValue::HasDelayedBinding() const
{
	return m_delayBinding && !m_destination.m_path.empty();
}

BlueStdResult Tr2ActionAnimateValue::EvaluateExpression( const char* expression, float& value ) const
{
	if( !m_controller )
	{
		return BlueStdResult( BLUE_STD_RESULT_RUNTIME_ERROR, "controller needs to be running when evaluating expressions" );
	}
	Tr2ControllerExpression expr;
	auto error = expr.SetExpr( expression, *m_controller, ModifyParser );
	if( !error.empty() )
	{
		return BlueStdResult( BLUE_STD_RESULT_VALUE_ERROR, error.c_str() );
	}
	s_stateTime = TimeAsFloat( m_lastSimTime - m_startTime );
	auto result = expr.Eval();
	if( !result.first )
	{
		return BlueStdResult( BLUE_STD_RESULT_RUNTIME_ERROR, "error evaluating expression" );
	}
	value = result.second;
	return BlueStdResult();
}