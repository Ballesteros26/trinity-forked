#pragma once
#ifndef Tr2QuaternionCurve_h
#define Tr2QuaternionCurve_h

// Trinity headers

#include "include/Tr2Curve.h"
#include "Utilities/Quaternion.h"

// --------------------------------------------------------------------------------------
// Description:
//   Tr2QuaternionKey is a specialization of Tr2Key for quaternions.
// SeeAlso:
//   Tr2Key, Tr2QuaternionCurve
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2QuaternionKey ):
	public IRoot,
	public Tr2Key<Quaternion>
{
public:
	// Constructor
	Tr2QuaternionKey( IRoot* lockobj = NULL );

	EXPOSE_TO_BLUE();
};
BLUE_DECLARE_VECTOR( Tr2QuaternionKey );


// --------------------------------------------------------------------------------------
// Description:
//   Tr2QuaternionCurve is a specialization of Tr2Curve for interpolating between 
//   quaternion keys.
// SeeAlso:
//   Tr2Curve, Tr2QuaternionKey
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2QuaternionCurve ) :
	public Tr2Curve<Tr2QuaternionKey, PTr2QuaternionKeyVector, Quaternion>
{
public:
	// Constructor
	Tr2QuaternionCurve( IRoot* lockobj = NULL );

	EXPOSE_TO_BLUE();

	// Sort keys into ascending order by key time
	void Sort();

	// Interpolate between quaternion key values
	Quaternion* Interpolate( Quaternion* out, Tr2QuaternionKey* startKey, Tr2QuaternionKey* endKey );

	void SetStartValue( const Quaternion& startValue ) { m_startValue = startValue; };
	void SetEndValue( const Quaternion& endValue ) { m_endValue = endValue; };
	void SetLength( float length ) { m_length = length; };


private:
	void AddKey_( float time, const Quaternion& value );
};

TYPEDEF_BLUECLASS( Tr2QuaternionCurve );
TYPEDEF_BLUECLASS( Tr2QuaternionKey );
#endif //Tr2QuaternionCurve_h
