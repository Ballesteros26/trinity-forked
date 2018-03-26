////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#pragma once

BLUE_DECLARE( Tr2Controller );


BLUE_INTERFACE( ITr2ControllerAction ) : public IRoot
{
	virtual void Link( Tr2Controller& controller ) {};
	virtual void Unlink() {};
	virtual void Start( Tr2Controller& controller ) {}
	virtual void Stop( Tr2Controller& controller ) {}
};

BLUE_DECLARE_IVECTOR( ITr2ControllerAction );
