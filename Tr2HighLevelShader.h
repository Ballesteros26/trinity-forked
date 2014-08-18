////////////////////////////////////////////////////////////
//
//    Created:   June 2010
//    Copyright: CCP 2010
//
#pragma once
#ifndef Tr2HighLevelShader_h
#define Tr2HighLevelShader_h

#include "Tr2DeviceResource.h"
#include "Tr2LowLevelShader.h"

BLUE_DECLARE( Tr2ShaderPermuteTag );
BLUE_DECLARE_VECTOR (Tr2ShaderPermuteTag );

BLUE_DECLARE( Tr2ShaderParameterDescription );
BLUE_DECLARE_VECTOR (Tr2ShaderParameterDescription );

BLUE_DECLARE( Tr2ShaderMaterial );  // for "CreateNewMaterial"
BLUE_DECLARE( Tr2HighLevelShader );
BLUE_DECLARE( Tr2LowLevelShader );

class Tr2ShaderSituation;

// --------------------------------------------------------------------------------------
// Description:
//   Tr2HighLevelShader holds a description of a shader and it's parameters.
//   It builds low level shaders given a Situation (which has been reduced to a vector of 
//   unsigned ints by this point)
//   It does not do any rendering
//   it does not hold material data
//   a High level shader can make a block of material data with defaults.
//   Tr2ShaderMaterial initially points to a High level shader by name, and then point 
//   via pointer on init.
// SeeAlso:
//   Tr2LowLevelShader, Tr2ShaderMaterial, Situation, Tr2EffectCache
// --------------------------------------------------------------------------------------
class Tr2HighLevelShader : public  IInitialize, public INotify, public Tr2DeviceResource
{
public:
	Tr2HighLevelShader( IRoot* lockobj = NULL );
	~Tr2HighLevelShader();

	EXPOSE_TO_BLUE();

	/////////////////////////////////////////////////////////////////////////////////////
	// INotify
	bool OnModified( Be::Var* val );
	
	/////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	virtual bool Initialize();

	/////////////////////////////////////////////////////////////////////////////////////
	// ITriDeviceResource
	void ReleaseResources( TriStorage s );
private:
	bool OnPrepareResources();
public:
	
	// Get hash of the high-level shader name
	unsigned int GetNameHash() const;

	// Get existing low-level shader (or create a new one), with the given situation
	Tr2LowLevelShader* GetOrCreateLowLevelShader( const Tr2ShaderSituation& situation );

	// Get the list of parameter descriptions
	const Tr2ShaderParameterDescriptionVector& GetParameterDescriptions( void ) const;

	// lets Shader materials register with my internal list, for updating register shadows.
	void RegisterShader( Tr2ShaderMaterial* shader );

	// if shader materials know they are chaning (due to text name switch), they need to unregister their old one here.
	void UnregisterShader( Tr2ShaderMaterial* shader );

	// Convert a permutation index back to a space-delimited string of situation flags
	//  (used in Jessica shader cache viewer thing)
	std::string PermuteIndexToString( const int permuteIndex ) const;

private:
	enum LoadingOption
	{
		SYNCRONOUS,
		ASYNCRONOUS,
	};
	// Loads the .fx file
	void LoadFXFile( LoadingOption );

	// Creates an array of #defines for the D3DX effect compiler
	Tr2EffectDefine* CreateDefinesFromSituation( const Tr2ShaderSituation& situation, 
		unsigned int& permuteIndexOut );

	// Rebuilds the low-level shaders after code change
	void RebuildLowLevelShadersAfterCodeChange();

	// Fetch an effect from the cache, creates effect if there is a miss
	void GetEffect( int permuteIndex, Tr2EffectDefine* currentDefines, Tr2ShaderCodeSource& codeSource, Tr2EffectDescription& effect ) const;

	std::vector<Tr2LowLevelShader*> GetLoadedShaders();
private:
	void CloseCacheFile();

	std::string m_name;
	std::string m_UIName; // string to show in the UI. if this is blank, then the shader is 'hidden' from the artists.
	unsigned int m_nameHash;

	std::string m_description;

	std::string m_shaderFilePath; // path to .FX file.

	// all permute tags.
	PTr2ShaderPermuteTagVector m_permuteTags;
	PTr2ShaderParameterDescriptionVector m_parameters;

	std::string m_renderTypeString;
	std::string m_alwaysCachePredicate;

	typedef std::map<uint32_t, Tr2LowLevelShaderPtr> ShaderStorageType;
	ShaderStorageType m_shaderStorage;

	// Map of low-level shader aliases (different permutations 
	// mapping to the same compiled shader)
	std::map<unsigned, Tr2LowLevelShaderPtr> m_shaderAliases;


	// a list of shaders that 'use' this HLS
	std::set<Tr2ShaderMaterial*> m_materialReferences; 

	// flag indicating that the last ReleaseResources was for managed
	// resources and we need to recreate the shader
	bool m_bReloadShader;

	// Per-permutation compiled file information
	struct FileRecord
	{
		// Compiled code offset into the file 
		unsigned offset;
		// Compiled code size
		unsigned size;
	};
	// Compiled shader file
	IBlueStreamPtr m_compiledFile;

	// Map from permuteIndex to offset+size in compiled
	// shader file
	std::map<unsigned, FileRecord> m_compiledPermutations;

	// String table from FXT file
	char* m_stringTable;
	// Length of string table
	unsigned m_stringTableSize;
	// Version of the shader file
	unsigned m_shaderFileVersion;
};

TYPEDEF_BLUECLASS(Tr2HighLevelShader);

BLUE_DECLARE_VECTOR(Tr2HighLevelShader);

#endif
