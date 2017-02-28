#include "StdAfx.h"

#include "Tr2ShaderParameterDescription.h"

#include "Shader/Parameter/Tr2FloatParameter.h"
#include "Shader/Parameter/Tr2Vector2Parameter.h"
#include "Shader/Parameter/Tr2Vector3Parameter.h"
#include "Shader/Parameter/Tr2Vector4Parameter.h"
#include "Shader/Parameter/TriTextureParameter.h"

// --------------------------------------------------------------------------------------
// Description:
//   Constructor for Tr2ShaderParameterDescription, which is the base class for all
//   typed parameter description classes.
// --------------------------------------------------------------------------------------
Tr2ShaderParameterDescription::Tr2ShaderParameterDescription( IRoot* lock )
	:m_permuteTagHash( 0 )
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialize function, inherited from IInitialize.  Sets up the parameter name and
//   computes the permute tag hash value from the parameter name.
// Return Value:
//   true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderParameterDescription::Initialize()
{
	// If artist name and shader name are the same, then just copy one to another if
	// parameter name was unset.
	if( m_parameterName.empty() )
	{
		if( !m_artistName.empty() )
		{
			m_parameterName = m_artistName;
		}
	}

	m_permuteTagHash = CcpHashFNV1( m_parameterName.c_str(), m_parameterName.length() );
	
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the computed permute hash tag.
// Return Value:
//   The permute hash tag
// --------------------------------------------------------------------------------------
unsigned int Tr2ShaderParameterDescription::GetPermuteTagHash() const
{
	return m_permuteTagHash;
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the shader parameter name.
// Return Value:
//   The shader parameter name
// --------------------------------------------------------------------------------------
const std::string& Tr2ShaderParameterDescription::GetShaderParameterName() const
{
	return m_parameterName;
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the optional artist-friendly parameter name.
// Return Value:
//   The artist-friendly parameter name
// --------------------------------------------------------------------------------------
const std::string& Tr2ShaderParameterDescription::GetArtistFriendlyName() const
{
	return m_artistName;
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the optional help text to display in the UI.
// Return Value:
//   The help text
// --------------------------------------------------------------------------------------
const std::string& Tr2ShaderParameterDescription::GetHelpText() const
{
	return m_helpText;
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ShaderFloat4Desc constructor.
// --------------------------------------------------------------------------------------
Tr2ShaderFloat4Desc::Tr2ShaderFloat4Desc( IRoot* obj ) :
	Tr2ShaderParameterDescription( obj ),
	m_defaultValue( 0.0f, 0.0f, 0.0f, 0.0f ),
	m_minimumValue( 0.0f, 0.0f, 0.0f, 0.0f ),
	m_maximumValue( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_isColor( false ),
	m_normalize( false ),
	m_thumbnailResPath(),
	m_valueToNameMethod(),
	m_nameToValueMethod()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialization function, inherited from IInitialize.
// Return Value:
//	 true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderFloat4Desc::Initialize()
{
	Tr2ShaderParameterDescription::Initialize();
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Constructs a new vector4 parameter from the default vector4 parameter value.  Note
//   that the created Tr2Vector4Parameter has a reference count of 1, so the caller
//   is responsible for calling Unlock (if needed).
// Return Value:
//   New default Tr2Vector4Parameter
// --------------------------------------------------------------------------------------
ITriEffectParameter* Tr2ShaderFloat4Desc::CreateDefaultParameter( void ) const
{
	OTr2Vector4Parameter* param = new OTr2Vector4Parameter();
	param->m_value = m_defaultValue;
	param->m_name = BlueSharedString( m_parameterName );

	return param;
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ShaderFloat3Desc constructor.
// --------------------------------------------------------------------------------------
Tr2ShaderFloat3Desc::Tr2ShaderFloat3Desc( IRoot* obj ) :
	Tr2ShaderParameterDescription( obj ),
	m_defaultValue( 0.0f, 0.0f, 0.0f ),
	m_minimumValue( 0.0f, 0.0f, 0.0f ),
	m_maximumValue( 1.0f, 1.0f, 1.0f ),
	m_isColor( false ),
	m_normalize( false ),
	m_thumbnailResPath(),
	m_valueToNameMethod(),
	m_nameToValueMethod()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialization function, inherited from IInitialize.
// Return Value:
//	 true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderFloat3Desc::Initialize()
{
	Tr2ShaderParameterDescription::Initialize();
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Constructs a new vector3 parameter from the default vector3 parameter value.  Note
//   that the created Tr2Vector3Parameter has a reference count of 1, so the caller
//   is responsible for calling Unlock (if needed).
// Return Value:
//   New default Tr2Vector3Parameter
// --------------------------------------------------------------------------------------
ITriEffectParameter* Tr2ShaderFloat3Desc::CreateDefaultParameter( void ) const
{
	OTr2Vector3Parameter* param = new OTr2Vector3Parameter();
	param->m_value = m_defaultValue;
	param->m_name = BlueSharedString( m_parameterName );

	return param;
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ShaderFloat2Desc constructor.
// --------------------------------------------------------------------------------------
Tr2ShaderFloat2Desc::Tr2ShaderFloat2Desc( IRoot* obj ) :
	Tr2ShaderParameterDescription( obj ),
	m_defaultValue( 0.0f, 0.0f ),
	m_minimumValue( 0.0f, 0.0f ),
	m_maximumValue( 1.0f, 1.0f ),
	m_normalize( false ),
	m_thumbnailResPath(),
	m_valueToNameMethod(),
	m_nameToValueMethod()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialization function, inherited from IInitialize.
// Return Value:
//	 true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderFloat2Desc::Initialize()
{
	Tr2ShaderParameterDescription::Initialize();
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Constructs a new vector2 parameter from the default vector2 parameter value.  Note
//   that the created Tr2Vector2Parameter has a reference count of 1, so the caller
//   is responsible for calling Unlock (if needed).
// Return Value:
//   New default Tr2Vector2Parameter
// --------------------------------------------------------------------------------------
ITriEffectParameter* Tr2ShaderFloat2Desc::CreateDefaultParameter( void ) const
{
	OTr2Vector2Parameter* param = new OTr2Vector2Parameter();
	param->m_value = m_defaultValue;
	param->m_name = BlueSharedString( m_parameterName );

	return param;
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ShaderFloatDesc constructor.
// --------------------------------------------------------------------------------------
Tr2ShaderFloatDesc::Tr2ShaderFloatDesc( IRoot* obj ) :
	Tr2ShaderParameterDescription( obj ),
	m_defaultValue( 0.0f ),
	m_minimumValue( 0.0f ),
	m_maximumValue( 1.0f ),
	m_normalize( false ),	
	m_thumbnailResPath(),
	m_valueToNameMethod(),
	m_nameToValueMethod()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialization function, inherited from IInitialize.
// Return Value:
//	 true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderFloatDesc::Initialize()
{
	Tr2ShaderParameterDescription::Initialize();
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Constructs a new float parameter from the default float parameter value.  Note
//   that the created Tr2FloatParameter has a reference count of 1, so the caller
//   is responsible for calling Unlock (if needed).
// Return Value:
//   New default Tr2FloatParameter
// --------------------------------------------------------------------------------------
ITriEffectParameter* Tr2ShaderFloatDesc::CreateDefaultParameter( void ) const
{
	OTr2FloatParameter* param = new OTr2FloatParameter();
	param->m_value = m_defaultValue;
	param->m_name = BlueSharedString( m_parameterName );

	return param;
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ShaderTexDesc constructor.
// --------------------------------------------------------------------------------------
Tr2ShaderTexDesc::Tr2ShaderTexDesc( IRoot* obj ) :
	Tr2ShaderParameterDescription( obj ),
	m_defaultValue()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Initialization function, inherited from IInitialize.
// Return Value:
//	 true always
// --------------------------------------------------------------------------------------
bool Tr2ShaderTexDesc::Initialize()
{
	Tr2ShaderParameterDescription::Initialize();
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Constructs a new texture parameter from the default texture parameter path.  Note
//   that the created TriTextureParameter has a reference count of 1, so the caller
//   is responsible for calling Unlock (if needed).
// Return Value:
//   New default TriTextureParameter
// --------------------------------------------------------------------------------------
ITriEffectParameter* Tr2ShaderTexDesc::CreateDefaultParameter( void ) const
{
	OTriTextureParameter* param = new OTriTextureParameter();
	param->SetParameterName( BlueSharedString( m_parameterName.c_str() ) );
	param->SetResourcePath( m_defaultValue.c_str() );

	return param;
}