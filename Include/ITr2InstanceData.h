////////////////////////////////////////////////////////////
//
//    Created:   December 2011
//    Copyright: CCP 2011
//

#pragma once
#ifndef ITr2InstanceData_H
#define ITr2InstanceData_H

class Tr2RenderContext;

BLUE_DECLARE_INTERFACE( ITr2InstanceData );

// --------------------------------------------------------------------------------------
// Description:
//   ITr2InstanceData is an interface for instance data provider that is intended to 
//   work with Tr2InstancedMesh class. Classes implementing ITr2InstanceData interface
//   are able to provide vertex buffers and declarations to be used as instance stream
//   for instanced rendering.
// See Also:
//   Tr2InstancedMesh, TriGeometryRes, Tr2SpriteParticleSystem
// --------------------------------------------------------------------------------------
BLUE_INTERFACE( ITr2InstanceData ) : public IRoot
{
	// --------------------------------------------------------------------------------------
	// Description:
	//   Query if the instance data is ready to be used. For example, for TriGeometryRes this
	//   is equivalent to checking if the resource is good.
	// Return Value:
	//   true if data is ready to be used
	//   false otherwise
	// --------------------------------------------------------------------------------------
	virtual bool IsInstanceDataReady() const = 0;

	// --------------------------------------------------------------------------------------
	// Description:
	//   Returns number of instance buffers available with this data.
	// Return Value:
	//   number of instance buffers for this object
	// --------------------------------------------------------------------------------------
	virtual unsigned int GetInstanceBufferCount() const = 0;

	// --------------------------------------------------------------------------------------
	// Description:
	//   Returns returns vertex declaration handle for the given instance buffer index.
	// Arguments:
	//   bufferIndex - instance buffer index
	// Return Value:
	//   vertex declaration handle
	// --------------------------------------------------------------------------------------
	virtual unsigned int GetInstanceBufferVertexDeclaration( unsigned int bufferIndex ) const = 0;

	// --------------------------------------------------------------------------------------
	// Description:
	//   Returns number of vertexes in the given instance buffer.
	// Arguments:
	//   bufferIndex - instance buffer index
	// Return Value:
	//   number of vertexes in the mesh
	// --------------------------------------------------------------------------------------
	virtual unsigned int GetInstanceBufferVertexCount( unsigned int bufferIndex ) const = 0;

	// --------------------------------------------------------------------------------------
	// Description:
	//   Returns vertex buffer with instance data.
	// Arguments:
	//   bufferIndex - instance buffer index
	//   buffer - (out) vertex buffer containing instance data (can be null)
	//   stride - (out) vertex stride for the vertex buffer
	// --------------------------------------------------------------------------------------
	virtual void GetVertexBuffer( unsigned int bufferIndex, Tr2BufferAL& buffer, unsigned& stride ) = 0;


	virtual bool GetInstanceBufferBoundingBox( unsigned int bufferIndex, Vector3& minBounds, Vector3& maxBounds ) const = 0;
};

#endif // ITr2InstanceData_H