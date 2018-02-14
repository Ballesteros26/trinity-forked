////////////////////////////////////////////////////////////
//
//    Created:   2018
//    Copyright: CCP 2018
//
#include "StdAfx.h"
#include "EveChildModifierBillboard3D.h"
#include "Tr2Renderer.h"
#include "EveChildModifierTransformCommon.h"

EveChildModifierBillboard3D::EveChildModifierBillboard3D( IRoot* lockobj )
{
}

EveChildModifierBillboard3D::~EveChildModifierBillboard3D()
{
}

Matrix EveChildModifierBillboard3D::ApplyTransform( const Matrix& transform ) const
{
	Matrix billboard = Billboard2D(transform);

	Matrix alignMat;
	float distCenter;
	Vector3 d;
	DistanceBase( billboard, alignMat, distCenter, d );

	return alignMat * billboard;
}