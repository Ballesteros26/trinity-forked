#pragma once
#ifndef Tr2TextureReference_H
#define Tr2TextureReference_H

#include "ITr2TextureProvider.h"

// -------------------------------------------------------------
// Description:
//   A simple blue-exposed class that stores a Tr2TextureAL. It
//   can be used to controll texture lifetime with reference
//   counting. It also allows using internal engine textures
//   with variable stores.
// -------------------------------------------------------------
BLUE_CLASS( Tr2TextureReference ): public ITr2TextureProvider
{
public:
	EXPOSE_TO_BLUE();

	Tr2TextureReference( IRoot* lockobj = nullptr );

	virtual Tr2TextureAL* GetTexture();
private:
	Tr2TextureAL m_texture;
};

TYPEDEF_BLUECLASS( Tr2TextureReference );

#endif