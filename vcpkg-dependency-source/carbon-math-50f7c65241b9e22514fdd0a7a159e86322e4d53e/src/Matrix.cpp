// Copyright (c) 2026 CCP Games

#include "Requirements.h"
#include "Matrix.h"

// --------------------------------------------------------------------------------------
Matrix Inverse( const Matrix& m )
{
	Vector4 v, vec[3];
	Matrix out;

	float det = Determinant( m );
	if( !det )
	{
		out = m;
	}
	else
	{
		for( int i = 0; i < 4; i++ )
		{
			float signedDet = ( i & 1 ) ? -1.f : 1.f;
			signedDet /= det;
			for( int j = 0; j < 4; j++ )
			{
				if( j != i )
				{
					int a = j;
					if( j > i )
					{
						a = a - 1;
					}
					vec[a].x = m.m[j][0];
					vec[a].y = m.m[j][1];
					vec[a].z = m.m[j][2];
					vec[a].w = m.m[j][3];
				}
			}
			v = Cross( vec[0], vec[1], vec[2] );
			out.m[0][i] = signedDet * v.x;
			out.m[1][i] = signedDet * v.y;
			out.m[2][i] = signedDet * v.z;
			out.m[3][i] = signedDet * v.w;
		}
	}
	return out;
}

// --------------------------------------------------------------------------------------
bool Inverse( Matrix& out, float& det, const Matrix& m )
{
	Vector4 v, vec[3];

	det = Determinant( m );
	if( !det )
	{
		return false;
	}
	else
	{
		out = Inverse( m );
	}
	return true;
}

// --------------------------------------------------------------------------------------
Matrix TransformationMatrix(
	const Vector3* scalingCenter,
	const Quaternion* scalingRotation,
	const Vector3* scaling,
	const Vector3* rotationCenter,
	const Quaternion *rotation,
	const Vector3* translation )
{
	Matrix m1, m2, m3, m4, m5, m6, m7, p1, p2, p3, p4, p5;
	Quaternion prc;
	Vector3 psc, pt;

	if( !scalingCenter )
	{
		psc.x = 0.0f;
		psc.y = 0.0f;
		psc.z = 0.0f;
	}
	else
	{
		psc.x = scalingCenter->x;
		psc.y = scalingCenter->y;
		psc.z = scalingCenter->z;
	}
	if( !rotationCenter )
	{
		prc.x = 0.0f;
		prc.y = 0.0f;
		prc.z = 0.0f;
	}
	else
	{
		prc.x = rotationCenter->x;
		prc.y = rotationCenter->y;
		prc.z = rotationCenter->z;
	}
	if( !translation )
	{
		pt.x = 0.0f;
		pt.y = 0.0f;
		pt.z = 0.0f;
	}
	else
	{
		pt.x = translation->x;
		pt.y = translation->y;
		pt.z = translation->z;
	}
	m1 = TranslationMatrix( -psc );
	if( !scalingRotation )
	{
		m2 = IdentityMatrix();
		m4 = IdentityMatrix();
	}
	else
	{
		m4 = RotationMatrix( *scalingRotation );
		m2 = Inverse( m4 );
	}
	if( !scaling )
	{
		m3 = IdentityMatrix();
	}
	else
	{
		m3 = ScalingMatrix( *scaling );
	}
	if( !rotation )
	{
		m6 = IdentityMatrix();
	}
	else
	{
		m6 = RotationMatrix( *rotation );
	}
	m5 = TranslationMatrix( psc.x - prc.x, psc.y - prc.y, psc.z - prc.z );
	m7 = TranslationMatrix( prc.x + pt.x, prc.y + pt.y, prc.z + pt.z );
	return m1 * m2 * m3 * m4 * m5 * m6 * m7;
}

// --------------------------------------------------------------------------------------
Matrix Transformation2DMatrix(
	const Vector2* scalingCenter,
	float scalingRotation,
	const Vector2* scaling,
	const Vector2* rotationCenter,
	float rotation,
	const Vector2* translation )
{
	Matrix m1, m2, m3, m4, m5, m6, m7;
	Quaternion prc;
	Vector3 psc, pt;

	if( !scalingCenter )
	{
		psc.x = 0.0f;
		psc.y = 0.0f;
	}
	else
	{
		psc.x = scalingCenter->x;
		psc.y = scalingCenter->y;
	}
	psc.z = 0.0f;
	if( !rotationCenter )
	{
		prc.x = 0.0f;
		prc.y = 0.0f;
	}
	else
	{
		prc.x = rotationCenter->x;
		prc.y = rotationCenter->y;
	}
	prc.z = 0.0f;
	if( !translation )
	{
		pt.x = 0.0f;
		pt.y = 0.0f;
	}
	else
	{
		pt.x = translation->x;
		pt.y = translation->y;
	}
	pt.z = 0.0f;
	m1 = TranslationMatrix( -psc.x, -psc.y, -psc.z );
	if( !scalingRotation )
	{
		m2 = IdentityMatrix();
		m4 = IdentityMatrix();
	}
	else
	{
		m4 = RotationZMatrix( scalingRotation );
		m2 = Inverse( m4 );
	}
	if( !scaling )
	{
		m3 = IdentityMatrix();
	}
	else
	{
		m3 = ScalingMatrix( scaling->x, scaling->y, 1.0f );
	}
	if( !rotation )
	{
		m6 = IdentityMatrix();
	}
	else
	{
		m6 = RotationZMatrix( rotation );
	}
	m5 = TranslationMatrix( psc.x - prc.x, psc.y - prc.y, psc.z - prc.z );
	m7 = TranslationMatrix( prc.x + pt.x, prc.y + pt.y, prc.z + pt.z );
	return m1 * m2 * m3 * m4 * m5 * m6 * m7;
}

// --------------------------------------------------------------------------------------
void Decompose( Vector3& scale, Quaternion& rotation, Vector3& translation, const Matrix& m )
{
	Matrix normalized;
	Vector3 vec;

	/*Compute the scaling part.*/
	vec.x = m.m[0][0];
	vec.y = m.m[0][1];
	vec.z = m.m[0][2];
	scale.x = Length( vec );

	vec.x = m.m[1][0];
	vec.y = m.m[1][1];
	vec.z = m.m[1][2];
	scale.y = Length( vec );

	vec.x = m.m[2][0];
	vec.y = m.m[2][1];
	vec.z = m.m[2][2];
	scale.z = Length( vec );

	/*Compute the translation part.*/
	translation.x = m.m[3][0];
	translation.y = m.m[3][1];
	translation.z = m.m[3][2];

	/*Let's calculate the rotation now*/
	if( ( scale.x == 0.0f ) || ( scale.y == 0.0f ) || ( scale.z == 0.0f ) )
	{
		rotation = Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
	}
	else
	{
		normalized.m[0][0] = m.m[0][0] / scale.x;
		normalized.m[0][1] = m.m[0][1] / scale.x;
		normalized.m[0][2] = m.m[0][2] / scale.x;
		normalized.m[1][0] = m.m[1][0] / scale.y;
		normalized.m[1][1] = m.m[1][1] / scale.y;
		normalized.m[1][2] = m.m[1][2] / scale.y;
		normalized.m[2][0] = m.m[2][0] / scale.z;
		normalized.m[2][1] = m.m[2][1] / scale.z;
		normalized.m[2][2] = m.m[2][2] / scale.z;

		rotation = RotationQuaternion( normalized );
	}
}
