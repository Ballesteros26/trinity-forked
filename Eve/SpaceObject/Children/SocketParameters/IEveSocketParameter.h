////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//

#pragma once
#ifndef IEveSocketParameter_H
#define IEveSocketParameter_H

BLUE_DECLARE( Tr2ExternalParameter );

BLUE_INTERFACE( IEveSocketParameter ) : public IRoot
{
	virtual const char* GetName() const { return ""; };
	virtual void SetName( const char* name ) {};

	virtual void ClearBindings() {}
	virtual bool BindToExternalParameter( const Tr2ExternalParameter& externalParameter ) { return true; };
	virtual void Reset() {};
	
	virtual void Propagate() {};
};

BLUE_DECLARE_IVECTOR( IEveSocketParameter );

#endif