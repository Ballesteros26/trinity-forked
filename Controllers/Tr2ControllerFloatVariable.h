////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#pragma once


BLUE_CLASS( Tr2ControllerFloatVariable ) : public IInitialize
{
public:
	enum Type
	{
		FLOAT,
		INTEGER,
		BOOLEAN,
	};

	Tr2ControllerFloatVariable( IRoot* lockobj = nullptr );

	EXPOSE_TO_BLUE();

	virtual bool Initialize();

	const std::string& GetName() const;
	float* GetPointerForParser();
private:
	std::string m_name;
	Type m_type;
	float m_value;
	float m_defaultValue;
};

TYPEDEF_BLUECLASS( Tr2ControllerFloatVariable );
BLUE_DECLARE_VECTOR( Tr2ControllerFloatVariable );
