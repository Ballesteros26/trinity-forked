#pragma once
#ifndef ITR2EFFECTVALUE_H
#define ITR2EFFECTVALUE_H

BLUE_INTERFACE( ITr2EffectValue ) : public IRoot
{
	// ----------------------------------------------------------------------------------
	// Description:
	//   Flags passed to resources in CopyValueToEffect (in "size" parameter).
	// ----------------------------------------------------------------------------------
	enum ResourceFlags
	{
		// Resource needs to be applied as sRGB texture
		RESOURCE_FLAG_SRGB	= 1,
		// Resource needs to be applied into UAV slot
		RESOURCE_FLAG_UAV	= 2,
	};

	// Copy the value to the effect using 'destHandle'.  For basic types 'destHandle'
	// is a memory address for the constant block being constructed for the effect.
	// For resources the 'destHandle' plays the role of the sampler index and size
	// is a bit field with ResourceFlags.
	// Size is passed down to potentially limit the number of bytes copied.
	// For example, a TriTransformParameter holds a 4x4 matrix but the shader
	// might use only a 2x2 portion of it. The fxc compiler is clever enough to only
	// give 2 constants to the variable in that case.
	// The size is in bytes.
	virtual void CopyValueToEffect(		Tr2RenderContextEnum::ShaderType inputType, 
										unsigned char* destHandle, 
										size_t size,
										Tr2RenderContext &renderContext ) const = 0;
};

TYPEDEF_BLUECLASS( ITr2EffectValue );

#endif