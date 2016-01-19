////////////////////////////////////////////////////////////
//
//    Created:   2016
//    Copyright: CCP 2016
//
#pragma once
#ifndef EveConnector_H
#define EveConnector_H

#include "include/ITriFunction.h"
#include "Eve/EveUpdateContext.h"

BLUE_DECLARE( EveCurveLineSet );
BLUE_DECLARE( EveConnector );
BLUE_DECLARE_VECTOR( EveConnector );

BLUE_CLASS( EveConnector ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();

	EveConnector( IRoot* lockobj = NULL );
	~EveConnector();

	enum ConnectorType { PointToPoint, XZ_CircleStraight, XZ_Circle, StraightAnchor, CurvedAnchor };

	void Update( EveUpdateContext& context );
	void AddLine( EveCurveLineSet* lineSet );

private:
	ITriVectorFunctionPtr m_sourceObject;
	ITriVectorFunctionPtr m_destObject;

	ConnectorType m_type;

	Vector3 m_sourcePosition;
	Vector3 m_destPosition;

	Color m_color;
	Color m_animationColor;
	
	float m_animationSpeed;
	float m_animationScale;
	float m_width;

	bool m_isAnimated;

	// Some helper functions(inline)
	void AnimateSegment( EveCurveLineSet* lineSet, int lineID );
	void AddCircle( EveCurveLineSet* lineSet, const Vector3& center, float radius );
	void AddStraightLine( EveCurveLineSet* lineSet, const Vector3& source, const Vector3& destination );
	void AddSpheredSegment( EveCurveLineSet* lineSet, const Vector3& p0, const Vector3& p1, const Vector3& center );
};

TYPEDEF_BLUECLASS( EveConnector );

#endif