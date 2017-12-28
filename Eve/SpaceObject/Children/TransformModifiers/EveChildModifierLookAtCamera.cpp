////////////////////////////////////////////////////////////
//
//    Created:   2017
//    Copyright: CCP 2017
//
#include "StdAfx.h"
#include "EveChildModifierLookAtCamera.h"
#include "Tr2Renderer.h"

EveChildModifierLookAtCamera::EveChildModifierLookAtCamera( IRoot* lockobj )
{
}

EveChildModifierLookAtCamera::~EveChildModifierLookAtCamera()
{
}

Matrix EveChildModifierLookAtCamera::ApplyTransform( const Matrix& transform ) const
{
		Matrix invView;
		Matrix result = transform;
		const Vector3& pos = transform.GetTranslation();
		const Vector3& camPos = Tr2Renderer::GetViewPosition();
		Vector3 up( 0.0f, 1.0f, 0.0f );
		invView = Transpose( LookAtMatrix( camPos, pos, up ) );

		float parentScaleX = Length( transform.GetX() );
		float parentScaleY = Length( transform.GetY() );
		float parentScaleZ = Length( transform.GetZ() );
		Vector3 finalScale( parentScaleX, parentScaleY, parentScaleZ );

		result._11 = invView._11 * finalScale.x;
		result._12 = invView._12 * finalScale.x;
		result._13 = invView._13 * finalScale.x;
		result._21 = invView._21 * finalScale.y;
		result._22 = invView._22 * finalScale.y;
		result._23 = invView._23 * finalScale.y;
		result._31 = invView._31 * finalScale.z;
		result._32 = invView._32 * finalScale.z;
		result._33 = invView._33 * finalScale.z;

		return result;
}