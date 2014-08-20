////////////////////////////////////////////////////////////////////////////////
//
// Created:		August 2014
// Copyright:	CCP 2014
//

#include "StdAfx.h"
#include "Tr2Texture2dLodParameter.h"
#include "Resources/Tr2LodResource.h"
#include "Resources/TriTextureRes.h"


Tr2Texture2dLodParameter::Tr2Texture2dLodParameter( IRoot* lockobj /*= nullptr */ )
{

}

TriTextureRes* Tr2Texture2dLodParameter::GetResource() const
{
	if( m_lodResource )
	{
		return dynamic_cast<TriTextureRes*>( m_lodResource->GetResource() );
	}

	return nullptr;
}
