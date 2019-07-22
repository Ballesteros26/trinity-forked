////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//

#pragma once
#ifndef EveSocketParameter_H
#define EveSocketParameter_H

#include "IEveSocketParameter.h"
#include "include/ITr2ValueBinding.h"


BLUE_DECLARE_IVECTOR( ITr2ValueBinding );


BLUE_CLASS( EveSocketParameterBase ) :
	public IEveSocketParameter
{
public:
	EXPOSE_TO_BLUE();

	EveSocketParameterBase( IRoot* lockobj = nullptr );
	~EveSocketParameterBase();

	virtual const char* GetName() const { return m_name.c_str(); }
	virtual void SetName( const char* name ) { m_name = name; }

	virtual void ClearBindings();
	virtual bool BindToExternalParameter( const Tr2ExternalParameter& externalParameter );

	virtual void Propagate();

protected:
	virtual bool ExtractDefault( const Tr2ExternalParameter& externalParameter ) = 0;

	std::string m_name;
	PITr2ValueBindingVector m_bindings;
};

TYPEDEF_BLUECLASS( EveSocketParameterBase );

#define SOCKET_PARAM_DECLARE( _className, _valueType )\
	BLUE_CLASS( _className ) :\
		public EveSocketParameterBase\
	{\
	public:\
		EXPOSE_TO_BLUE();\
		virtual void ClearBindings();\
		virtual void Reset();\
	protected:\
		virtual bool ExtractDefault( const Tr2ExternalParameter& externalParameter );\
	private:\
		_valueType m_value;\
		std::vector<_valueType> m_defaults;\
	};\
	TYPEDEF_BLUECLASS( ##_className## );

SOCKET_PARAM_DECLARE( EveSocketParameterFloat, float );
SOCKET_PARAM_DECLARE( EveSocketParameterVector2, Vector2 );
SOCKET_PARAM_DECLARE( EveSocketParameterVector3, Vector3 );
SOCKET_PARAM_DECLARE( EveSocketParameterVector4, Vector4 );
SOCKET_PARAM_DECLARE( EveSocketParameterColor, Color );

#endif