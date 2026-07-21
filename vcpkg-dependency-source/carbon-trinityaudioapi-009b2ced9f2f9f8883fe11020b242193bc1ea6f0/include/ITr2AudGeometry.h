// Copyright © 2026 CCP ehf.
//
//    Description:
//      An interface that sets Geometry (meshes) from trinity to carbon-audio

#pragma once

#ifndef ITr2AudGeometry_h_
#define ITr2AudGeometry_h_

#include <vector>
#include <cstdint>

struct Tr2AudGeometryData
{
	std::vector<Vector3> m_vertices;
	std::vector<uint32_t> m_indices;
	Vector3 m_minBounds;
	Vector3 m_maxBounds;
};

BLUE_INTERFACE( ITr2AudGeometry ) :
	public IRoot
{
	virtual void SetGeometry(
		uint64_t geometrySetId,
		uint64_t instanceId,
		const Tr2AudGeometryData& geometryData,
		const Matrix& worldTransform ) = 0;

	virtual void SetGeometryTransform(
		uint64_t geometrySetId,
		uint64_t instanceId,
		const Matrix& worldTransform ) = 0;

	virtual void RemoveGeometry(
		uint64_t geometrySetId,
		uint64_t instanceId ) = 0;
};

#endif