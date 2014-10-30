////////////////////////////////////////////////////////////
//
//    Created:   October 2014
//    Copyright: CCP 2014
//
#pragma once
#ifndef EveDistanceField_H
#define EveDistanceField_H

BLUE_DECLARE( EveCamera );
BLUE_DECLARE( EveUpdateContext );
BLUE_DECLARE_INTERFACE( ITriVectorFunction );
BLUE_DECLARE_VECTOR( ITriVectorFunction );
BLUE_DECLARE( TriCurveSet );

// --------------------------------------------------------------------------------------
// Description:
//   WIP
// --------------------------------------------------------------------------------------
BLUE_CLASS( EveDistanceField ):
	public IRoot
{
public:
	EveDistanceField( IRoot* lockobj = 0 );

	EXPOSE_TO_BLUE();

	void Update( const EveUpdateContext& updateContext );
private:
	PITriVectorFunctionVector m_objects;

	TriCurveSetPtr m_curveSet;

	EveCameraPtr m_camera;

	double m_distance;
	// Adjust how long it takes to settle on a new value when zooming out
	float m_timeAdjustmentSecondsOut;
	// Adjust how long it takes to settle on a new value when zooming in
	float m_timeAdjustmentSecondsIn;
};
	
TYPEDEF_BLUECLASS( EveDistanceField );

#endif