////////////////////////////////////////////////////////////
//
//    Created:   October 2012
//    Copyright: CCP 2012
//

#include "StdAfx.h"
#include "Tr2GeometryBufferParameter.h"
#include "include/ITr2GpuBuffer.h"
#include "Shader/Tr2Shader.h"

// --------------------------------------------------------------------------------------
// Description:
//   Tr2GeometryBufferParameter default constructor
// --------------------------------------------------------------------------------------
Tr2GeometryBufferParameter::Tr2GeometryBufferParameter( IRoot* lockobj )
	:m_meshIndex( 0 ),
	m_initialCount( -1 ),
	m_isUsedByEffect( false )
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Tr2GeometryBufferParameter destructor
// --------------------------------------------------------------------------------------
Tr2GeometryBufferParameter::~Tr2GeometryBufferParameter()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements IInitialize interface. Loads geometry resource.
// Return Value:
//   true always
// --------------------------------------------------------------------------------------
bool Tr2GeometryBufferParameter::Initialize()
{
	if( !m_resourcePath.empty() )
	{
		m_gpuBuffer.Unlock();
		BeResMan->GetResourceW( m_resourcePath.c_str(), L"", BlueInterfaceIID<ITr2GpuBuffer>(), (void**)&m_gpuBuffer );
	}
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements INotify interface. Allows the object to respond to parameter changes 
//   generated in Python. Reloads geometry if geometry path has changed.  
// Arguments:
//   value - The Blue-exposed parameter that changed
// Return Value:
//   true always
// --------------------------------------------------------------------------------------
bool Tr2GeometryBufferParameter::OnModified( Be::Var* val )
{
	if( IsMatch( val, m_resourcePath ) )
	{
		m_gpuBuffer.Unlock();
		Initialize();
		RebuildEffectHandles( m_cachedEffect );
	}
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements ITriEffectResourceParameter interface. Returns effect parameter name.
// Return Value:
//   Effect parameter name
// --------------------------------------------------------------------------------------
const char* Tr2GeometryBufferParameter::GetParameterName() const
{
	return m_name.c_str();
}

unsigned Tr2GeometryBufferParameter::GetHashValue( unsigned startingHash ) const
{
	if( !m_resourcePath.empty() )
	{
		startingHash = CcpHashFNV1( m_resourcePath.c_str(), m_resourcePath.length() * sizeof( wchar_t ), startingHash );
	}
	auto name = m_name.c_str();
	return CcpHashFNV1( &name, sizeof( name ), startingHash );
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements ITriEffectResourceParameter interface. Called when owner effect changes.
//   Stores owner effect and updates "is used" flag.
// Arguments:
//   effectRes - Owner effect resource
// --------------------------------------------------------------------------------------
void Tr2GeometryBufferParameter::RebuildEffectHandles( Tr2Shader* effectRes )
{
	m_cachedEffect = effectRes;

	if ( m_name.empty() || !effectRes || !effectRes->GetResource( m_name.c_str() ) )
	{
		m_isUsedByEffect = false;
		return;
	}

	m_isUsedByEffect = true;
	if( auto annotations = effectRes->GetParameterAnnotations( m_name.c_str() ) )
	{
		auto found = std::find_if( 
			annotations->begin(), 
			annotations->end(), 
			[&]( const Tr2EffectParameterAnnotation& x ) 
			{ 
				return x.type == Tr2EffectParameterAnnotation::INT && strcmp( x.name, m_name.c_str() ) == 0;
		} );
		if( found != annotations->end() )
		{
			m_initialCount = found->intValue;
		}
	}
	
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements ITriEffectResourceParameter interface. Applies paramter value to effect.
// Arguments:
//   inputType - Shader type to apply resource to
//   destHandle - Slot number
//   resourceFlags - union of ITr2EffectValue::ResourceFlags
//   renderContext - current render context
// --------------------------------------------------------------------------------------
void Tr2GeometryBufferParameter::CopyValueToEffect(		Tr2RenderContextEnum::ShaderType inputType, 
														unsigned char* destHandle, 
														size_t resourceFlags,
														Tr2RenderContext &renderContext ) const
{
	if( !m_gpuBuffer )
	{
		return;
	}
	bool isUav = ( resourceFlags & RESOURCE_FLAG_UAV ) != 0;
	unsigned int ix = *destHandle;
	uint32_t initialCount = reinterpret_cast<uint32_t*>( destHandle )[1];
	Tr2GpuBufferAL* buffer = m_gpuBuffer->GetGpuBuffer( m_meshIndex );
	if( !buffer )
	{
		return;
	}
	if( isUav )
	{
		renderContext.SetUav( inputType, ix, *buffer, initialCount == -1 ? m_initialCount : initialCount );
	}
	else
	{
		renderContext.m_esm.ApplyShaderBuffer( inputType, ix, *buffer );
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Checks if the parameter data is valid. Used for debugging.
// Return value:
//   true If paramter contains valid geometry or UAV buffer
//   false Otherwise
// --------------------------------------------------------------------------------------
bool Tr2GeometryBufferParameter::IsValid() const
{
	return m_gpuBuffer && m_gpuBuffer->GetGpuBuffer( m_meshIndex );
}

// --------------------------------------------------------------------------------------
void Tr2GeometryBufferParameter::SetGpuBuffer( ITr2GpuBuffer* buffer )
{
	m_resourcePath = L"";
	m_gpuBuffer = buffer;
}