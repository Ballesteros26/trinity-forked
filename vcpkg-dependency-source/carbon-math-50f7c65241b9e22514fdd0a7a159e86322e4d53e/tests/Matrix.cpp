// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"

// disable division by 0 warning: we are doing it on purpose
#pragma warning(disable: 4723)

#define EXPECT_MATRIX_EQ( expected, actual )								\
{																			\
	for( int i = 0; i < 4; ++i )											\
		for( int j = 0; j < 4; ++j )										\
			EXPECT_FLOAT_EQ( ( expected ).m[i][j], ( actual ).m[i][j] );	\
}

TEST( Matrix, Constructors ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( i + 1 ) * 10 + ( j + 1 ), mat1.m[i][j] );
		}
	}

	Matrix mat2( mat1 );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( i + 1 ) * 10 + ( j + 1 ), mat1.m[i][j] );
		}
	}

	float elements[] = { 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f };

    Matrix mat3( XMMATRIX{elements} );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( i + 1 ) * 10 + ( j + 1 ), mat1.m[i][j] );
		}
	}
}

TEST( Matrix, ElementAccess ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( i + 1 ) * 10 + ( j + 1 ), mat1( i, j ) );
		}
	}

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			mat1( i, j ) *= i + j;
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * ( i + j ), mat1( i, j ) );
		}
	}
}

TEST( Matrix, Additions ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		55.f, 56.f, 57.f, 58.f,
		65.f, 66.f, 67.f, 68.f,
		75.f, 76.f, 77.f, 78.f,
		85.f, 86.f, 87.f, 88.f );

	mat1 += mat2;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			float element = float( i + 1 ) * 10 + ( j + 1 ) + ( i + 5 ) * 10 + ( j + 5 );
			EXPECT_EQ( element, mat1.m[i][j] );
		}
	}

	Matrix mat3 = mat1 + mat2;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat1.m[i][j] + mat2.m[i][j], mat3.m[i][j] );
		}
	}
}

TEST( Matrix, Subtractions ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	float elements[16];
	for( int i = 0; i < 16; ++i )
	{
		elements[i] = 16.f - float( i );
	}
    Matrix mat2( XMMATRIX{elements} );

	mat1 -= mat2;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( i + 1 ) * 10 + ( j + 1 ) - mat2.m[i][j], mat1.m[i][j] );
		}
	}

	Matrix mat3 = mat1 - mat2;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat1.m[i][j] - mat2.m[i][j], mat3.m[i][j] );
		}
	}
}

TEST( Matrix, Scaling ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	mat1 *= 0.5f;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * 0.5f, mat1.m[i][j] );
		}
	}

	Matrix mat2 = mat1 * 10.f;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * 5.f, mat2.m[i][j] );
		}
	}

	mat2 /= 2.f;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * 2.5f, mat2.m[i][j] );
		}
	}

	Matrix mat3 = mat2 / 0.1f;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * 25.f, mat3.m[i][j] );
		}
	}

	mat2 *= 0.f;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( 0.f, mat2.m[i][j] );
		}
	}

	mat3 /= mat2._11;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_FALSE( CcpIsFinite( mat3.m[i][j] ) );
		}
	}

	Matrix mat4( 3.f * mat1 );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( ( ( i + 1 ) * 10 + ( j + 1 ) ) * 1.5f, mat4.m[i][j] );
		}
	}
}

TEST( Matrix, Signs ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	Matrix mat2( +mat1 );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat1.m[i][j], mat2.m[i][j] );
		}
	}

	Matrix mat3( -mat1 );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( -mat1.m[i][j], mat3.m[i][j] );
		}
	}
}

TEST( Matrix, Comparisons ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat3( 
		55.f, 56.f, 57.f, 58.f,
		65.f, 66.f, 67.f, 68.f,
		75.f, 76.f, 77.f, 78.f,
		85.f, 86.f, 87.f, 88.f );

	EXPECT_TRUE( mat1 == mat2 );
	EXPECT_FALSE( mat1 == mat3 );
	EXPECT_FALSE( mat1 != mat2 );
	EXPECT_TRUE( mat2 != mat3 );

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			Matrix mat4( mat1 );
			mat4.m[i][j] += 1.f;
			EXPECT_FALSE( mat4 == mat1 );
			EXPECT_TRUE( mat4 != mat1 );
		}
	}
}

TEST( Matrix, Translation ) 
{
	const Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	EXPECT_TRUE( Vector3( 41.f, 42.f, 43.f ) == mat1.GetTranslation() );
	mat2.GetTranslation().x = 123.f;
	EXPECT_TRUE( Vector3( 123.f, 42.f, 43.f ) == mat2.GetTranslation() );
}

TEST( Matrix, XRow ) 
{
	const Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	EXPECT_TRUE( Vector3( 11.f, 12.f, 13.f ) == mat1.GetX() );
	mat2.GetX().x = 123.f;
	EXPECT_TRUE( Vector3( 123.f, 12.f, 13.f ) == mat2.GetX() );
}

TEST( Matrix, YRow ) 
{
	const Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	EXPECT_TRUE( Vector3( 21.f, 22.f, 23.f ) == mat1.GetY() );
	mat2.GetY().x = 123.f;
	EXPECT_TRUE( Vector3( 123.f, 22.f, 23.f ) == mat2.GetY() );
}

TEST( Matrix, ZRow ) 
{
	const Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	EXPECT_TRUE( Vector3( 31.f, 32.f, 33.f ) == mat1.GetZ() );
	mat2.GetZ().x = 123.f;
	EXPECT_TRUE( Vector3( 123.f, 32.f, 33.f ) == mat2.GetZ() );
}


TEST( Matrix, MatrixIdentity ) 
{
	Matrix mat2 = IdentityMatrix();
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( i == j ? 1.f : 0.f, mat2.m[i][j] );
		}
	}
}

TEST( Matrix, MatrixDeterminant ) 
{
	const Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	EXPECT_EQ( 0.f, Determinant( mat1 ) );

	EXPECT_EQ( 1.f, Determinant( IdentityMatrix() ) );
}

TEST( Matrix, MatrixTranspose ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );

	Matrix mat3 = Transpose( mat1 );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat1.m[i][j], mat3.m[j][i] );
		}
	}
}

TEST( Matrix, MatrixMultiply ) 
{
	Matrix mat1( 
		11.f, 12.f, 13.f, 14.f,
		21.f, 22.f, 23.f, 24.f,
		31.f, 32.f, 33.f, 34.f,
		41.f, 42.f, 43.f, 44.f );
	Matrix mat2( 
		55.f, 56.f, 57.f, 58.f,
		65.f, 66.f, 67.f, 68.f,
		75.f, 76.f, 77.f, 78.f,
		85.f, 86.f, 87.f, 88.f );

	Matrix mat12(
		3550.f, 3600.f, 3650.f, 3700.f, 
		6350.f, 6440.f, 6530.f, 6620.f, 
		9150.f, 9280.f, 9410.f, 9540.f, 
		11950.f, 12120.f, 12290.f, 12460.f );
	Matrix mat11(
		1350.f, 1400.f, 1450.f, 1500.f, 
		2390.f, 2480.f, 2570.f, 2660.f, 
		3430.f, 3560.f, 3690.f, 3820.f, 
		4470.f, 4640.f, 4810.f, 4980.f );

	Matrix mat3 = mat1 * mat2;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat12.m[i][j], mat3.m[i][j] );
		}
	}

	Matrix mat4 = mat1;
	mat4 *= mat4;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( mat11.m[i][j], mat4.m[i][j] );
		}
	}
}

TEST( Matrix, MatrixInverse ) 
{
	Matrix mat1 = IdentityMatrix();
	Matrix mat2;
	float det1;
	EXPECT_TRUE( Inverse( mat2, det1, mat1 ) );
	EXPECT_EQ( 1.f, det1 );
	EXPECT_TRUE( mat2 == mat1 );

	Matrix mat3;
	EXPECT_TRUE( Inverse( mat3, mat1 ) );
	EXPECT_TRUE( mat3 == mat1 );

	Matrix mat4 = Inverse( mat1 );
	EXPECT_TRUE( mat4 == mat1 );

	Matrix mat6(
		9.45227336883545f, 0.f, 0.f, 0.f, 
		0.f, 6.616590976715088f, 0.f, 0.f, 
		0.f, 0.f, 1.001001000404358f, 1.f, 
		0.f, 0.f, -0.10010010004043579f, 0.f );
	Matrix mat6inv(
		0.10579466074705124f, 0.f, 0.f, 0.f, 
		0.f, 0.15113522112369537f, 0.f, 0.f, 
		0.f, 0.f, 0.f, -9.989999771118164f, 
		0.f, 0.f, 1.f, 10.f );

	EXPECT_TRUE( Inverse( mat2, det1, mat6 ) );
	EXPECT_EQ( 6.260443210601807f, det1 );
	EXPECT_TRUE( mat2 == mat6inv );

	EXPECT_TRUE( Inverse( mat3, mat6 ) );
	EXPECT_TRUE( mat3 == mat6inv );

	mat4 = Inverse( mat6 );
	EXPECT_TRUE( mat4 == mat6inv );
}


TEST( Matrix, MatrixScaling ) 
{
	Matrix mat2 = ScalingMatrix( 4.f, 3.f, 2.f );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			EXPECT_EQ( i == j ? float( 4 - i ) : 0.f, mat2.m[i][j] );
		}
	}
}

TEST( Matrix, MatrixTranslation ) 
{
	Matrix mat2 = TranslationMatrix( 4.f, 3.f, 2.f );
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			if( i == 3 )
			{
				EXPECT_EQ( 4.f - j, mat2.m[i][j] );
			}
			else
			{
				EXPECT_EQ( i == j ? 1.f : 0.f, mat2.m[i][j] );
			}
		}
	}
}

TEST( Matrix, MatrixRotationX ) 
{
	Matrix rot03(
		1.f, 0.f, 0.f, 0.f, 
		0.f, 0.9553365111351013f, 0.29552021622657776f, 0.f, 
		0.f, -0.29552021622657776f, 0.9553365111351013f, 0.f, 
		0.f, 0.f, 0.f, 1.f );

	EXPECT_MATRIX_EQ( rot03, RotationXMatrix( 0.3f ) );
}

TEST( Matrix, MatrixRotationY ) 
{
	Matrix rot04(
		0.9210609793663025f, 0.f, -0.3894183337688446f, 0.f, 
		0.f, 1.f, 0.f, 0.f, 
		0.3894183337688446f, 0.f, 0.9210609793663025f, 0.f, 
		0.f, 0.f, 0.f, 1.f );

	EXPECT_MATRIX_EQ( rot04, RotationYMatrix( 0.4f ) );
}

TEST( Matrix, MatrixRotationZ ) 
{
	Matrix rot05(
		0.8775825500488281f, 0.4794255495071411f, 0.f, 0.f, 
		-0.4794255495071411f, 0.8775825500488281f, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f, 
		0.f, 0.f, 0.f, 1.f );

	EXPECT_MATRIX_EQ( rot05, RotationZMatrix( 0.5f ) );
}

TEST( Matrix, MatrixRotationAxis ) 
{
	Vector3 axis( 1.f, 2.f, 3.f );
	Matrix result(
		0.9585267305374146f, 0.24332378804683685f, -0.148391455411911f, 0.f, 
		-0.23056279122829437f, 0.9680975079536438f, 0.0981225893f, 0.f, 
		0.16753295063972473f, -0.059839606285095215f, 0.9840487241744995f, 0.f, 
		0.f, 0.f, 0.f, 1.f );

	EXPECT_MATRIX_EQ( result, RotationMatrix( axis, 0.3f ) );
}

TEST( Matrix, MatrixRotationQuaternion ) 
{
	Quaternion q2( 1.f, 2.f, 3.f, 4.f );
	Matrix result(
		-25.0f, 28.0f, -10.0f, 0.0f, 
		-20.0f, -19.0f, 20.0f, 0.0, 
		22.0f, 4.0f, -9.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0 );

	EXPECT_MATRIX_EQ( result, RotationMatrix( q2 ) );
}
