// Copyright © 2011 CCP ehf.

#include "StdAfx.h"

#include <cmath>

#include "Tr2ProjectBoundingBoxBracket.h"
#include "include/ITr2BoundingBox.h"
#include "Tr2Renderer.h"
#include "TriViewport.h"
#include "Sprite2d/Tr2Sprite2dContainer.h"
#include "Utilities/BoundingBox.h"
#include "Utilities/BoundingSphere.h"
#include "include/ITr2DebugRenderer.h"

extern ITr2DebugRendererPtr g_debugRenderer;

namespace
{
const float CLIP_EPSILON = 1e-5f;

struct ClipPoint
{
	float x;
	float y;
	float z;
	float w;
};

struct ProjectedBounds
{
	float x;
	float y;
	float z;
	float width;
	float height;
	bool extendsOffscreen;
	bool coversViewport;
};

bool IsFinite( float value )
{
	return std::isfinite( value );
}

bool IsFinite( const ClipPoint& point )
{
	return IsFinite( point.x ) && IsFinite( point.y ) && IsFinite( point.z ) && IsFinite( point.w );
}

ClipPoint TransformPointToClip( const Vector3& point, const Matrix& viewProjection )
{
	return ClipPoint{
		point.x * viewProjection._11 + point.y * viewProjection._21 + point.z * viewProjection._31 + viewProjection._41,
		point.x * viewProjection._12 + point.y * viewProjection._22 + point.z * viewProjection._32 + viewProjection._42,
		point.x * viewProjection._13 + point.y * viewProjection._23 + point.z * viewProjection._33 + viewProjection._43,
		point.x * viewProjection._14 + point.y * viewProjection._24 + point.z * viewProjection._34 + viewProjection._44
	};
}

ClipPoint Lerp( const ClipPoint& a, const ClipPoint& b, float t )
{
	return ClipPoint{
		a.x + ( b.x - a.x ) * t,
		a.y + ( b.y - a.y ) * t,
		a.z + ( b.z - a.z ) * t,
		a.w + ( b.w - a.w ) * t
	};
}

bool AreAllOutsidePlane( const ClipPoint* points, float ( *planeDistance )( const ClipPoint& ) )
{
	for( int i = 0; i < 8; ++i )
	{
		if( planeDistance( points[i] ) >= 0.0f )
		{
			return false;
		}
	}
	return true;
}

float DistanceToLeftPlane( const ClipPoint& point )
{
	return point.x + point.w;
}
float DistanceToRightPlane( const ClipPoint& point )
{
	return point.w - point.x;
}
float DistanceToBottomPlane( const ClipPoint& point )
{
	return point.y + point.w;
}
float DistanceToTopPlane( const ClipPoint& point )
{
	return point.w - point.y;
}
float DistanceToNearPlane( const ClipPoint& point )
{
	return point.z;
}
float DistanceToFarPlane( const ClipPoint& point )
{
	return point.w - point.z;
}

bool IsTriviallyOutsideFrustum( const ClipPoint* points )
{
	return AreAllOutsidePlane( points, DistanceToLeftPlane ) ||
		AreAllOutsidePlane( points, DistanceToRightPlane ) ||
		AreAllOutsidePlane( points, DistanceToBottomPlane ) ||
		AreAllOutsidePlane( points, DistanceToTopPlane ) ||
		AreAllOutsidePlane( points, DistanceToNearPlane ) ||
		AreAllOutsidePlane( points, DistanceToFarPlane );
}

bool CanPerspectiveDivide( const ClipPoint& point )
{
	return IsFinite( point ) && fabsf( point.w ) > CLIP_EPSILON;
}

void AddIfProjectable( const ClipPoint& point, std::vector<ClipPoint>& points )
{
	if( point.z >= 0.0f && CanPerspectiveDivide( point ) )
	{
		points.push_back( point );
	}
}

void AddNearPlaneIntersection( const ClipPoint& a, const ClipPoint& b, std::vector<ClipPoint>& points )
{
	if( ( a.z < 0.0f ) == ( b.z < 0.0f ) )
	{
		return;
	}

	float denominator = a.z - b.z;
	if( fabsf( denominator ) <= CLIP_EPSILON )
	{
		return;
	}

	float t = a.z / denominator;
	ClipPoint point = Lerp( a, b, t );
	if( CanPerspectiveDivide( point ) )
	{
		points.push_back( point );
	}
}

bool ProjectClipPoint( const ClipPoint& point, const TriViewport& viewport, Vector3& projected )
{
	if( !CanPerspectiveDivide( point ) )
	{
		return false;
	}

	float reciprocalW = 1.0f / point.w;
	projected.x = viewport.x + ( 1.0f + point.x * reciprocalW ) * 0.5f * viewport.width;
	projected.y = viewport.y + ( 1.0f - point.y * reciprocalW ) * 0.5f * viewport.height;
	projected.z = viewport.minZ + point.z * reciprocalW * ( viewport.maxZ - viewport.minZ );
	return IsFinite( projected.x ) && IsFinite( projected.y ) && IsFinite( projected.z );
}

bool ProjectBoundingBoxToViewport( const Vector3& bbMin, const Vector3& bbMax, const Matrix& viewProjection, const TriViewport& viewport, ProjectedBounds& bounds )
{
	Vector3 corners[8];
	corners[0] = bbMin;
	corners[1] = Vector3( bbMin.x, bbMin.y, bbMax.z );
	corners[2] = Vector3( bbMax.x, bbMin.y, bbMin.z );
	corners[3] = Vector3( bbMax.x, bbMin.y, bbMax.z );
	corners[4] = bbMax;
	corners[5] = Vector3( bbMax.x, bbMax.y, bbMin.z );
	corners[6] = Vector3( bbMin.x, bbMax.y, bbMax.z );
	corners[7] = Vector3( bbMin.x, bbMax.y, bbMin.z );

	ClipPoint clipCorners[8];
	for( int i = 0; i < 8; ++i )
	{
		clipCorners[i] = TransformPointToClip( corners[i], viewProjection );
		if( !IsFinite( clipCorners[i] ) )
		{
			return false;
		}
	}

	if( IsTriviallyOutsideFrustum( clipCorners ) )
	{
		return false;
	}

	std::vector<ClipPoint> projectablePoints;
	projectablePoints.reserve( 20 );
	for( int i = 0; i < 8; ++i )
	{
		AddIfProjectable( clipCorners[i], projectablePoints );
	}

	static const int EDGES[12][2] = {
		{ 0, 1 }, { 1, 3 }, { 3, 2 }, { 2, 0 }, { 7, 6 }, { 6, 4 }, { 4, 5 }, { 5, 7 }, { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 }
	};

	for( int i = 0; i < 12; ++i )
	{
		AddNearPlaneIntersection( clipCorners[EDGES[i][0]], clipCorners[EDGES[i][1]], projectablePoints );
	}

	if( projectablePoints.empty() )
	{
		return false;
	}

	Vector3 projected;
	bool hasProjectedPoint = false;
	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;
	float maxX = 0.0f;
	float maxY = 0.0f;

	for( const ClipPoint& point : projectablePoints )
	{
		if( !ProjectClipPoint( point, viewport, projected ) )
		{
			continue;
		}

		if( !hasProjectedPoint )
		{
			minX = maxX = projected.x;
			minY = maxY = projected.y;
			minZ = projected.z;
			hasProjectedPoint = true;
		}
		else
		{
			minX = std::min( minX, projected.x );
			maxX = std::max( maxX, projected.x );
			minY = std::min( minY, projected.y );
			maxY = std::max( maxY, projected.y );
			minZ = std::min( minZ, projected.z );
		}
	}

	if( !hasProjectedPoint )
	{
		return false;
	}

	float width = maxX - minX;
	float height = maxY - minY;
	if( !IsFinite( width ) || !IsFinite( height ) || width <= 0.0f || height <= 0.0f )
	{
		return false;
	}

	float viewportLeft = static_cast<float>( viewport.x );
	float viewportTop = static_cast<float>( viewport.y );
	float viewportRight = viewportLeft + static_cast<float>( viewport.width );
	float viewportBottom = viewportTop + static_cast<float>( viewport.height );

	bounds.x = minX;
	bounds.y = minY;
	bounds.z = minZ;
	bounds.width = width;
	bounds.height = height;
	bounds.extendsOffscreen = minX < viewportLeft || minY < viewportTop || maxX > viewportRight || maxY > viewportBottom;
	bounds.coversViewport = minX <= viewportLeft && minY <= viewportTop && maxX >= viewportRight && maxY >= viewportBottom;
	return true;
}

enum class SphereProjection
{
	PROJECTED,
	EMPTY,
	UNSUPPORTED
};

struct AxisBounds
{
	float minSlope;
	float maxSlope;
	bool hasBounds;
};

// Exact near-clipped silhouette bounds of a sphere in one screen axis, as view-space
// slopes (axis / forward depth). The per-axis 2D tangent construction is exact for the
// 3D silhouette: the cross-axis coordinate drops out of the tangency condition at the
// axis extremes.
AxisBounds ComputeSphereAxisBounds( float a, float b, float r, float nearDepth )
{
	AxisBounds result{ 0.0f, 0.0f, false };

	float candidates[4];
	int candidateCount = 0;

	const float m2 = a * a + b * b - r * r;
	const float denominator = b * b - r * r;
	if( m2 > 0.0f && fabsf( denominator ) > CLIP_EPSILON )
	{
		const float m = sqrtf( m2 );
		const float tangents[2] = { ( a * b - r * m ) / denominator, ( a * b + r * m ) / denominator };
		for( float u : tangents )
		{
			// A tangent whose touch point lies behind the near plane does not bound
			// the visible silhouette; the near-plane circle takes over there.
			const float tangentDepth = ( a * u + b ) / ( 1.0f + u * u );
			if( IsFinite( u ) && tangentDepth >= nearDepth )
			{
				candidates[candidateCount++] = u;
			}
		}
	}

	const float nearOffset = nearDepth - b;
	const float nearCircleSq = r * r - nearOffset * nearOffset;
	if( nearCircleSq > 0.0f )
	{
		const float nearCircleRadius = sqrtf( nearCircleSq );
		candidates[candidateCount++] = ( a - nearCircleRadius ) / nearDepth;
		candidates[candidateCount++] = ( a + nearCircleRadius ) / nearDepth;
	}

	if( candidateCount == 0 )
	{
		return result;
	}

	result.minSlope = result.maxSlope = candidates[0];
	for( int i = 1; i < candidateCount; ++i )
	{
		result.minSlope = std::min( result.minSlope, candidates[i] );
		result.maxSlope = std::max( result.maxSlope, candidates[i] );
	}
	result.hasBounds = true;
	return result;
}

// Requires a rigid view transform and a symmetric, non-oblique perspective projection;
// reports UNSUPPORTED otherwise so the caller can fall back to the box path.
SphereProjection ProjectBoundingSphereToViewport( const Vector4& sphere, const Matrix& view, const Matrix& projection, const TriViewport& viewport, ProjectedBounds& bounds )
{
	const float radius = sphere.w;
	if( radius <= 0.0f )
	{
		return SphereProjection::UNSUPPORTED;
	}

	const float handedness = projection._34;
	if( fabsf( fabsf( handedness ) - 1.0f ) > CLIP_EPSILON ||
		projection._11 <= 0.0f || projection._22 <= 0.0f || projection._33 == 0.0f ||
		projection._12 != 0.0f || projection._13 != 0.0f || projection._14 != 0.0f ||
		projection._21 != 0.0f || projection._23 != 0.0f || projection._24 != 0.0f ||
		projection._31 != 0.0f || projection._32 != 0.0f ||
		projection._41 != 0.0f || projection._42 != 0.0f || projection._44 != 0.0f )
	{
		return SphereProjection::UNSUPPORTED;
	}

	const float nearDepth = -handedness * projection._43 / projection._33;
	if( !IsFinite( nearDepth ) || nearDepth <= 0.0f )
	{
		return SphereProjection::UNSUPPORTED;
	}

	const Vector3 centerView = TransformCoord( BoundingSphereGetCenter( sphere ), view );
	const float forward = handedness * centerView.z;
	if( !IsFinite( centerView.x ) || !IsFinite( centerView.y ) || !IsFinite( forward ) )
	{
		return SphereProjection::UNSUPPORTED;
	}

	if( forward + radius < nearDepth )
	{
		return SphereProjection::EMPTY;
	}

	const float farDenominator = projection._33 - handedness;
	if( fabsf( farDenominator ) > CLIP_EPSILON )
	{
		const float farDepth = -handedness * projection._43 / farDenominator;
		if( IsFinite( farDepth ) && farDepth > nearDepth && forward - radius > farDepth )
		{
			return SphereProjection::EMPTY;
		}
	}

	const AxisBounds xBounds = ComputeSphereAxisBounds( centerView.x, forward, radius, nearDepth );
	const AxisBounds yBounds = ComputeSphereAxisBounds( centerView.y, forward, radius, nearDepth );
	if( !xBounds.hasBounds || !yBounds.hasBounds )
	{
		return SphereProjection::EMPTY;
	}

	const float minNdcX = projection._11 * xBounds.minSlope;
	const float maxNdcX = projection._11 * xBounds.maxSlope;
	const float minNdcY = projection._22 * yBounds.minSlope;
	const float maxNdcY = projection._22 * yBounds.maxSlope;
	if( !IsFinite( minNdcX ) || !IsFinite( maxNdcX ) || !IsFinite( minNdcY ) || !IsFinite( maxNdcY ) )
	{
		return SphereProjection::UNSUPPORTED;
	}

	if( maxNdcX < -1.0f || minNdcX > 1.0f || maxNdcY < -1.0f || minNdcY > 1.0f )
	{
		return SphereProjection::EMPTY;
	}

	const float viewportLeft = static_cast<float>( viewport.x );
	const float viewportTop = static_cast<float>( viewport.y );
	const float viewportWidth = static_cast<float>( viewport.width );
	const float viewportHeight = static_cast<float>( viewport.height );

	const float minX = viewportLeft + ( 1.0f + minNdcX ) * 0.5f * viewportWidth;
	const float maxX = viewportLeft + ( 1.0f + maxNdcX ) * 0.5f * viewportWidth;
	const float minY = viewportTop + ( 1.0f - maxNdcY ) * 0.5f * viewportHeight;
	const float maxY = viewportTop + ( 1.0f - minNdcY ) * 0.5f * viewportHeight;

	const float width = maxX - minX;
	const float height = maxY - minY;
	if( !IsFinite( width ) || !IsFinite( height ) || width <= 0.0f || height <= 0.0f )
	{
		return SphereProjection::EMPTY;
	}

	const float centerDistance = Length( centerView );
	const float nearestDepth = std::max( nearDepth, centerDistance - radius );
	const float ndcZ = ( handedness * nearestDepth * projection._33 + projection._43 ) / nearestDepth;
	const float clampedNdcZ = std::min( 1.0f, std::max( 0.0f, ndcZ ) );

	const float viewportRight = viewportLeft + viewportWidth;
	const float viewportBottom = viewportTop + viewportHeight;

	bounds.x = minX;
	bounds.y = minY;
	bounds.z = viewport.minZ + clampedNdcZ * ( viewport.maxZ - viewport.minZ );
	bounds.width = width;
	bounds.height = height;
	bounds.extendsOffscreen = minX < viewportLeft || minY < viewportTop || maxX > viewportRight || maxY > viewportBottom;
	bounds.coversViewport = minX <= viewportLeft && minY <= viewportTop && maxX >= viewportRight && maxY >= viewportBottom;
	return SphereProjection::PROJECTED;
}

bool ClampToScreenMargin( const TriViewport& viewport, float margin, float& x, float& y, float& width, float& height )
{
	if( margin <= 0.0f )
	{
		return true;
	}

	float left = static_cast<float>( viewport.x ) + margin;
	float top = static_cast<float>( viewport.y ) + margin;
	float right = static_cast<float>( viewport.x ) + static_cast<float>( viewport.width ) - margin;
	float bottom = static_cast<float>( viewport.y ) + static_cast<float>( viewport.height ) - margin;

	float minX = std::max( x, left );
	float minY = std::max( y, top );
	float maxX = std::min( x + width, right );
	float maxY = std::min( y + height, bottom );

	if( maxX <= minX || maxY <= minY )
	{
		return false;
	}

	x = minX;
	y = minY;
	width = maxX - minX;
	height = maxY - minY;
	return true;
}
}


Tr2ProjectBoundingBoxBracket::Tr2ProjectBoundingBoxBracket( IRoot* lockobj /*= NULL */ ) :
	m_minProjectedWidth( 0.0f ),
	m_minProjectedHeight( 0.0f ),
	m_maxProjectedWidth( 0.0f ),
	m_maxProjectedHeight( 0.0f ),
	m_projectedX( 0.0f ),
	m_projectedY( 0.0f ),
	m_projectedZ( 0.0f ),
	m_projectedWidth( 0.0f ),
	m_projectedHeight( 0.0f ),
	m_integerCoordinates( true ),
	m_screenMargin( 0.0f ),
	m_cameraDistance( 0 ),
	m_isProjectionValid( false ),
	m_containsCamera( false ),
	m_extendsOffscreen( false ),
	m_coversViewport( false )
{
}


void Tr2ProjectBoundingBoxBracket::UpdateValue( double time )
{
	if( !m_object )
	{
		SetEmptyProjection();
		return;
	}

	if( !m_object->IsBoundingBoxReady() )
	{
		SetEmptyProjection();
		return;
	}

	Vector4 sphere( 0.0f, 0.0f, 0.0f, 0.0f );
	const bool hasSphere = m_object->GetWorldBoundingSphere( sphere ) && sphere.w > 0.0f;

	Vector3 bbMin, bbMax;
	bool hasBox = false;
	Vector3 center;
	if( hasSphere )
	{
		center = BoundingSphereGetCenter( sphere );
	}
	else
	{
		if( !m_object->GetWorldBoundingBox( bbMin, bbMax ) )
		{
			SetEmptyProjection();
			return;
		}
		hasBox = true;
		center = ( bbMax + bbMin ) * 0.5f;
	}

	Vector3 d = Tr2Renderer::GetViewPosition() - center;
	m_cameraDistance = Length( d );

	const TriViewport& viewport = Tr2Renderer::GetViewport();
	const bool cameraInside = hasSphere ? m_cameraDistance <= sphere.w : BoundingBoxIsInside( bbMin, bbMax, Tr2Renderer::GetViewPosition() );
	if( cameraInside )
	{
		m_projectedX = static_cast<float>( viewport.x );
		m_projectedY = static_cast<float>( viewport.y );
		m_projectedZ = viewport.minZ;
		m_projectedWidth = static_cast<float>( viewport.width );
		m_projectedHeight = static_cast<float>( viewport.height );
		if( !ClampToScreenMargin( viewport, m_screenMargin, m_projectedX, m_projectedY, m_projectedWidth, m_projectedHeight ) )
		{
			SetEmptyProjection();
			return;
		}
		m_isProjectionValid = true;
		m_containsCamera = true;
		m_extendsOffscreen = true;
		m_coversViewport = true;
		UpdateBracket();
		return;
	}

	Matrix viewProjection = Tr2Renderer::GetViewTransform() * Tr2Renderer::GetProjectionTransform();
	ProjectedBounds projectedBounds;
	bool hasProjection = false;
	if( hasSphere )
	{
		switch( ProjectBoundingSphereToViewport( sphere, Tr2Renderer::GetViewTransform(), Tr2Renderer::GetProjectionTransform(), viewport, projectedBounds ) )
		{
		case SphereProjection::PROJECTED:
			hasProjection = true;
			break;
		case SphereProjection::EMPTY:
			SetEmptyProjection();
			return;
		case SphereProjection::UNSUPPORTED:
			break;
		}
	}

	if( !hasProjection )
	{
		if( !hasBox && !m_object->GetWorldBoundingBox( bbMin, bbMax ) )
		{
			SetEmptyProjection();
			return;
		}
		if( !ProjectBoundingBoxToViewport( bbMin, bbMax, viewProjection, viewport, projectedBounds ) )
		{
			SetEmptyProjection();
			return;
		}
	}

	m_projectedX = projectedBounds.x;
	m_projectedY = projectedBounds.y;
	m_projectedZ = projectedBounds.z;
	m_projectedWidth = projectedBounds.width;
	m_projectedHeight = projectedBounds.height;
	m_containsCamera = false;
	m_extendsOffscreen = projectedBounds.extendsOffscreen;
	m_coversViewport = projectedBounds.coversViewport;

	float centerX = m_projectedX + m_projectedWidth * 0.5f;
	float centerY = m_projectedY + m_projectedHeight * 0.5f;
	if( m_maxProjectedWidth > 0.0f || m_maxProjectedHeight > 0.0f )
	{
		// Bounded brackets are anchored on the projected 3d box center, not the projected
		// rect center, unless the box center is behind the near plane.
		ClipPoint clipCenter = TransformPointToClip( center, viewProjection );
		Vector3 projectedCenter;
		if( clipCenter.z >= 0.0f && clipCenter.w > 0.0f && ProjectClipPoint( clipCenter, viewport, projectedCenter ) )
		{
			centerX = projectedCenter.x;
			centerY = projectedCenter.y;
		}
	}

	if( m_minProjectedWidth > 0.0f && m_projectedWidth < m_minProjectedWidth )
	{
		m_projectedWidth = m_minProjectedWidth;
	}
	else if( m_maxProjectedWidth > 0.0f && m_projectedWidth > m_maxProjectedWidth )
	{
		m_projectedWidth = m_maxProjectedWidth;
	}

	if( m_minProjectedHeight > 0.0f && m_projectedHeight < m_minProjectedHeight )
	{
		m_projectedHeight = m_minProjectedHeight;
	}
	else if( m_maxProjectedHeight > 0.0f && m_projectedHeight > m_maxProjectedHeight )
	{
		m_projectedHeight = m_maxProjectedHeight;
	}

	m_projectedX = centerX - m_projectedWidth * 0.5f;
	m_projectedY = centerY - m_projectedHeight * 0.5f;

	if( m_integerCoordinates )
	{
		m_projectedX = floor( m_projectedX + 0.5f );
		m_projectedY = floor( m_projectedY + 0.5f );
		m_projectedWidth = floor( m_projectedWidth + 0.5f );
		m_projectedHeight = floor( m_projectedHeight + 0.5f );
	}

	if( !IsFinite( m_projectedX ) || !IsFinite( m_projectedY ) || !IsFinite( m_projectedWidth ) || !IsFinite( m_projectedHeight ) || m_projectedWidth <= 0.0f || m_projectedHeight <= 0.0f )
	{
		SetEmptyProjection();
		return;
	}

	if( !ClampToScreenMargin( viewport, m_screenMargin, m_projectedX, m_projectedY, m_projectedWidth, m_projectedHeight ) )
	{
		SetEmptyProjection();
		return;
	}

	m_isProjectionValid = true;
	UpdateBracket();

	if( g_debugRenderer )
	{
		int x = static_cast<int>( m_projectedX );
		int y = static_cast<int>( m_projectedY );
		g_debugRenderer->Printf( x, y, 0xffffffff, "%S", m_name.c_str() );
		y += 16;
		g_debugRenderer->Printf( x, y, 0xffffffff, "(%5.2f, %5.2f)", m_projectedWidth, m_projectedHeight );
	}
}

void Tr2ProjectBoundingBoxBracket::SetEmptyProjection()
{
	m_projectedX = 0.0f;
	m_projectedY = 0.0f;
	m_projectedZ = 0.0f;
	m_projectedWidth = 0.0f;
	m_projectedHeight = 0.0f;
	m_isProjectionValid = false;
	m_containsCamera = false;
	m_extendsOffscreen = false;
	m_coversViewport = false;

	UpdateBracket();
}

void Tr2ProjectBoundingBoxBracket::UpdateBracket()
{
	if( m_bracket )
	{
		m_bracket->SetDisplayX( m_projectedX );
		m_bracket->SetDisplayY( m_projectedY );
		m_bracket->SetDisplayWidth( m_projectedWidth );
		m_bracket->SetDisplayHeight( m_projectedHeight );
	}
}
