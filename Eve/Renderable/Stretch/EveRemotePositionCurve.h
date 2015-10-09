////////////////////////////////////////////////////////////
//
//    Created:   October 2015
//    Copyright: CCP 2015
//

#ifndef _EVEREMOTEPOSITIONCURVE_H_
#define _EVEREMOTEPOSITIONCURVE_H_

#include "include/ITriFunction.h"

BLUE_CLASS( EveRemotePositionCurve ) :
	public ITriVectorFunction
{

public:
	EXPOSE_TO_BLUE();

	EveRemotePositionCurve( IRoot* lockobj = nullptr );
	~EveRemotePositionCurve();

	/////////////////////////////////////////////////////////////////////////////////////
	// ITriVectorFunction
	void UpdateValue( double time ) { Vector3 v; Update( &v, time ); }
	Vector3* Update( Vector3* in, Be::Time time );
	Vector3* Update( Vector3* in, double time );
	Vector3* GetValueAt( Vector3* in, Be::Time time );
	Vector3* GetValueAt( Vector3* in, double time );
	Vector3* GetValueDotAt( Vector3* in, Be::Time time );
	Vector3* GetValueDotAt( Vector3* in, double time );
	Vector3* GetValueDoubleDotAt( Vector3* in, Be::Time time );
	Vector3* GetValueDoubleDotAt( Vector3* in, double time );
	Vector3d* InterpolatedPosition( Vector3d* out, Be::Time time );

private:
};

TYPEDEF_BLUECLASS( EveRemotePositionCurve );

#endif 
