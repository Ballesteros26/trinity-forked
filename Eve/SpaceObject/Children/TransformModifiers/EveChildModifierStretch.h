////////////////////////////////////////////////////////////
//
//    Created:   2020
//    Copyright: CCP 2020
//
#pragma once

#include "IEveChildTransformModifier.h"
#include "include/ITriFunction.h"

BLUE_CLASS( EveChildModifierStretch ) :
	public IEveChildTransformModifier
{
public:
	EXPOSE_TO_BLUE();

	EveChildModifierStretch( IRoot* lockobj = NULL );
	~EveChildModifierStretch();

	Matrix ApplyTransform( const Matrix& transform, size_t boneCount, const granny_matrix_3x4* bones ) const;
	void SetSource( ITriVectorFunction* source );
	void SetDest( ITriVectorFunction* dest );

private:
	ITriVectorFunctionPtr m_source;
	ITriVectorFunctionPtr m_dest;

};

TYPEDEF_BLUECLASS( EveChildModifierStretch );
