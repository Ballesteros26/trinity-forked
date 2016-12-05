#include "StdAfx.h"
#include "Tr2TextureReference.h"

Tr2TextureReference::Tr2TextureReference( IRoot* )
{
}

Tr2TextureAL* Tr2TextureReference::GetTexture()
{
	return &m_texture;
}
