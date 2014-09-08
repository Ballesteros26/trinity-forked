#include "StdAfx.h"
#include "TriTexture2DParameter.h"
#include "Resources/TriTextureRes.h"

BLUE_DEFINE( TriTexture2DParameter );

extern Be::VarChooser SamplerStateChooser_AddressMode[];
extern Be::VarChooser SamplerStateChooser_FilterMode[];
extern Be::VarChooser SamplerStateChooser_MipFilterMode[];

const Be::ClassInfo* TriTexture2DParameter::ExposeToBlue()
{
	EXPOSURE_BEGIN( TriTexture2DParameter, "" )
		MAP_INTERFACE( ITriEffectParameter )
		MAP_INTERFACE( ITriEffectResourceParameter )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( INotify )
		MAP_INTERFACE( ICopierCustomAssignment )

		////////////////////////////////////////////////////////////////////////////
		//	name
		MAP_ATTRIBUTE
		( 
			"name",    
			m_name,    
			"na", 
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)
		////////////////////////////////////////////////////////////////////////////
		//	resource path (texture / buffer)
		// TODO: Add valid chooser
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"resourcePath",	
			m_resourcePath,				
			"Resource path to .x file", 
			Be::READWRITE | Be::NOTIFY | Be::PERSIST,
			NULL
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE
		(    
			"resource",
			m_resource,
			"na",
			Be::READ
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE("usedByCurrentTechnique", m_isUsedByEffect, "na", Be::READ)
		MAP_ATTRIBUTE("usedByCurrentEffect", m_isUsedByEffect, "na", Be::READ)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE( 
			"useAllOverrides",
			m_useOverrides,
			"use all(!) the sampler-state overrides listed below",
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"addressUMode",
			m_overrideAddressUMode,
			"changes u address mode of this texture",
			Be::READWRITE | Be::NOTIFY | Be::ENUM | Be::PERSIST,
			SamplerStateChooser_AddressMode
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"addressVMode",
			m_overrideAddressVMode,
			"changes v address mode of this texture",
			Be::READWRITE | Be::NOTIFY | Be::ENUM | Be::PERSIST,
			SamplerStateChooser_AddressMode
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"addressWMode",
			m_overrideAddressWMode,
			"changes w address mode of this texture",
			Be::READWRITE | Be::NOTIFY | Be::ENUM | Be::PERSIST,
			SamplerStateChooser_AddressMode
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"filterMode",
			m_overrideFilterMode,
			"changes filter mode of this texture",
			Be::READWRITE | Be::NOTIFY | Be::ENUM | Be::PERSIST,
			SamplerStateChooser_FilterMode
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"mipFilterMode",
			m_overrideMipFilterMode,
			"changes filter mode between mip-levels of this texture",
			Be::READWRITE | Be::NOTIFY | Be::ENUM | Be::PERSIST,
			SamplerStateChooser_MipFilterMode
		)

		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE( 
			"mipmapLodBias",
			m_overrideMipmapLodBias,
			"changes the mipmap-level-bias of this texture (0.0 is default!)",
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE( 
			"maxMipLevel",
			m_overrideMaxMipLevel,
			"max miplevel used by this texture (0 is default!)",
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE( 
			"maxAnisotropy",
			m_overrideMaxAnisotropy,
			"max anisotropy used by this texture (default 4)",
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)
		////////////////////////////////////////////////////////////////////////////
		MAP_ATTRIBUTE( 
			"Srgb",
			m_overrideSrgb,
			"sample as sRGB? (default false)",
			Be::READWRITE | Be::NOTIFY | Be::PERSIST
		)

		MAP_METHOD_AND_WRAP( "SetResource", SetResource, "Takes a TriTextureRes and sets it directly, without using a resourcePath.")
		MAP_METHOD_AND_WRAP( "GetResourcePath", GetResourcePath, "Returns the respath to the currently used texture. Might be LOD dependent." )

	EXPOSURE_END( )
}

