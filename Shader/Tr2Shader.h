#pragma once
#ifndef Tr2Shader_h
#define Tr2Shader_h

#include "Tr2EffectDescription.h"


BLUE_CLASS( Tr2Shader ) : public IRoot
{
public:
	Tr2Shader( IRoot* lockobj = NULL );
	~Tr2Shader();

	EXPOSE_TO_BLUE();

	uint32_t GetPassCount() const;
	void ApplyAllStateForPass( uint32_t passIndex, Tr2RenderContext &renderContext ) const;
	uint32_t GetShaderTypeMask() const;
	const Tr2EffectDescription& GetEffectDescription() const;

	void ApplyRenderStates( uint32_t passIndex, Tr2RenderContext &renderContext ) const;
	void ApplySamplerStates( uint32_t passIndex, Tr2RenderContextEnum::ShaderType type, Tr2RenderContext &renderContext ) const;
	void ApplyShader( uint32_t passIndex, Tr2RenderContextEnum::ShaderType type, Tr2RenderContext &renderContext ) const;

	const Tr2EffectConstant* GetConstant( const char* name ) const;
	const Tr2EffectResource* GetResource( const char* name ) const;
	const Tr2EffectParameterAnnotationMap* GetParameterAnnotations( const char* parameterName ) const;

	unsigned int GetSortValue() const;
	
	Tr2EffectDescription& GetEffect();
	void ProcessEffect();

private:
	unsigned int m_sortValue;
	Tr2EffectDescription m_effect;
};

TYPEDEF_BLUECLASS( Tr2Shader );

#endif
