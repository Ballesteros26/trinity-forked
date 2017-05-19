////////////////////////////////////////////////////////////
//
//    Created:   May 2017
//    Copyright: CCP 2017
//

#pragma once

#include "Include/ITriFunction.h"
#include "Include/ITriCurveLength.h"
#include "Tr2CurveScalar.h"


BLUE_CLASS( Tr2CurveEulerRotation ) :
	public ITriCurveLength,
	public ITriFunction
{
public:
	Tr2CurveEulerRotation( IRoot* lockobj = nullptr );

	EXPOSE_TO_BLUE();

	virtual void UpdateValue( double time );

	virtual float Length();

	Quaternion GetValue( double time ) const;
private:
	std::string m_name;

	PTr2CurveScalar m_yaw;
	PTr2CurveScalar m_pitch;
	PTr2CurveScalar m_roll;

	Quaternion m_currentValue;
};

TYPEDEF_BLUECLASS( Tr2CurveEulerRotation );