// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"

// disable division by 0 warning: we are doing it on purpose
#pragma warning(disable: 4723)

#define EXPECT_QUATERNION_EQ( expected, actual )			\
	{														\
		EXPECT_FLOAT_EQ( ( expected ).x, ( actual ).x );	\
		EXPECT_FLOAT_EQ( ( expected ).y, ( actual ).y );	\
		EXPECT_FLOAT_EQ( ( expected ).z, ( actual ).z );	\
		EXPECT_FLOAT_EQ( ( expected ).w, ( actual ).w );	\
	}

TEST( Quaternion, Constructors ) 
{
	Quaternion q1( 1.f, 2.f, 3.f, 4.f );
	EXPECT_EQ( 1.f, q1.x );
	EXPECT_EQ( 2.f, q1.y );
	EXPECT_EQ( 3.f, q1.z );
	EXPECT_EQ( 4.f, q1.w );

	Quaternion q2( q1 );
	EXPECT_EQ( 1.f, q2.x );
	EXPECT_EQ( 2.f, q2.y );
	EXPECT_EQ( 3.f, q2.z );
	EXPECT_EQ( 4.f, q2.w );
}

TEST( Quaternion, Additions ) 
{
	Quaternion a( 1.f, 2.f, 3.f, 4.f );
	Quaternion b( 4.f, 5.f, 6.f, 7.f );

	a += b;
	EXPECT_EQ( 5.f, a.x );
	EXPECT_EQ( 7.f, a.y );
	EXPECT_EQ( 9.f, a.z );
	EXPECT_EQ( 11.f, a.w );

	a += a;
	EXPECT_EQ( 10.f, a.x );
	EXPECT_EQ( 14.f, a.y );
	EXPECT_EQ( 18.f, a.z );
	EXPECT_EQ( 22.f, a.w );

	Quaternion c = a + b;
	EXPECT_EQ( 14.f, c.x );
	EXPECT_EQ( 19.f, c.y );
	EXPECT_EQ( 24.f, c.z );
	EXPECT_EQ( 29.f, c.w );
}

TEST( Quaternion, Subtractions ) 
{
	Quaternion a( 12.f, 23.f, 4.f, 45.f );
	Quaternion b( 4.f, 5.f, 6.f, 7.f );

	a -= b;
	EXPECT_EQ( 8.f, a.x );
	EXPECT_EQ( 18.f, a.y );
	EXPECT_EQ( -2.f, a.z );
	EXPECT_EQ( 38.f, a.w );

	Quaternion c = a - b;
	EXPECT_EQ( 4.f, c.x );
	EXPECT_EQ( 13.f, c.y );
	EXPECT_EQ( -8.f, c.z );
	EXPECT_EQ( 31.f, c.w );
	
	Quaternion d = a - a;
	EXPECT_EQ( 0.f, d.x );
	EXPECT_EQ( 0.f, d.y );
	EXPECT_EQ( 0.f, d.z );
	EXPECT_EQ( 0.f, d.w );
}

TEST( Quaternion, Scaling ) 
{
	Quaternion a( 1.f, 2.f, 3.f, 4.f );

	a *= 0.5f;
	EXPECT_EQ( 0.5f, a.x );
	EXPECT_EQ( 1.f, a.y );
	EXPECT_EQ( 1.5f, a.z );
	EXPECT_EQ( 2.f, a.w );

	Quaternion b = a * -10.f;
	EXPECT_EQ( -5.f, b.x );
	EXPECT_EQ( -10.f, b.y );
	EXPECT_EQ( -15.f, b.z );
	EXPECT_EQ( -20.f, b.w );

	b /= -2.f;
	EXPECT_EQ( 2.5f, b.x );
	EXPECT_EQ( 5.f, b.y );
	EXPECT_EQ( 7.5f, b.z );
	EXPECT_EQ( 10.f, b.w );

	Quaternion c = b / 0.1f;
	EXPECT_EQ( 25.f, c.x );
	EXPECT_EQ( 50.f, c.y );
	EXPECT_EQ( 75.f, c.z );
	EXPECT_EQ( 100.f, c.w );

	b *= 0.f;
	EXPECT_EQ( 0.f, b.x );
	EXPECT_EQ( 0.f, b.y );
	EXPECT_EQ( 0.f, b.z );
	EXPECT_EQ( 0.f, b.w );

	c /= b.x;
	EXPECT_FALSE( CcpIsFinite( c.x ) );
	EXPECT_FALSE( CcpIsFinite( c.y ) );
	EXPECT_FALSE( CcpIsFinite( c.z ) );
	EXPECT_FALSE( CcpIsFinite( c.w ) );

	Quaternion d( 3.f * a );
	EXPECT_EQ( 1.5f, d.x );
	EXPECT_EQ( 3.f, d.y );
	EXPECT_EQ( 4.5f, d.z );
	EXPECT_EQ( 6.f, d.w );
}

TEST( Quaternion, Signs ) 
{
	Quaternion a( 1.f, 2.f, 3.f, 4.f );

	Quaternion b( +a );
	EXPECT_EQ( 1.f, b.x );
	EXPECT_EQ( 2.f, b.y );
	EXPECT_EQ( 3.f, b.z );
	EXPECT_EQ( 4.f, b.w );

	Quaternion c( -a );
	EXPECT_EQ( -1.f, c.x );
	EXPECT_EQ( -2.f, c.y );
	EXPECT_EQ( -3.f, c.z );
	EXPECT_EQ( -4.f, c.w );
}

TEST( Quaternion, Comparisons ) 
{
	EXPECT_TRUE( Quaternion( 1.f, 2.f, 3.f, 4.f ) == Quaternion( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Quaternion( 1.f, 2.f, 3.f, 4.f ) == Quaternion( 3.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Quaternion( 1.f, 2.f, 3.f, 4.f ) == Quaternion( 1.f, 0.f, 3.f, 4.f ) );
	EXPECT_FALSE( Quaternion( 1.f, 2.f, 3.f, 4.f ) == Quaternion( 1.f, 2.f, 0.f, 4.f ) );
	EXPECT_FALSE( Quaternion( 1.f, 0.f, 3.f, 4.f ) == Quaternion( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Quaternion( 1.f, 0.f, 3.f, 4.f ) == Quaternion( 1.f, 2.f, 3.f, 5.f ) );

	EXPECT_FALSE( Quaternion( 1.f, 2.f, 3.f, 4.f ) != Quaternion( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Quaternion( 1.f, 2.f, 3.f, 4.f ) != Quaternion( 3.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Quaternion( 1.f, 2.f, 3.f, 4.f ) != Quaternion( 1.f, 0.f, 3.f, 4.f ) );
	EXPECT_TRUE( Quaternion( 1.f, 2.f, 3.f, 4.f ) != Quaternion( 1.f, 2.f, 0.f, 4.f ) );
	EXPECT_TRUE( Quaternion( 1.f, 0.f, 3.f, 4.f ) != Quaternion( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Quaternion( 1.f, 0.f, 3.f, 4.f ) != Quaternion( 1.f, 2.f, 3.f, 5.f ) );
}

TEST( Quaternion, Multiplication ) 
{
	Quaternion q1( 1.f, 2.f, 3.f, 4.f );
	Quaternion q2( 2.4f, 7.1f, -3.1f, 0.2f );
	Quaternion result(37.2999992f, 18.4999981f, -14.0999994f, -6.50000095f);

	Quaternion q5 = q1 * q2;
	EXPECT_QUATERNION_EQ( result, q5 );

	Quaternion q6 = q1;
	q6 *= q2;
	EXPECT_QUATERNION_EQ( result, q6 );
}

TEST( Quaternion, QuaternionLength ) 
{
	Quaternion q1( 1.f, 2.f, 3.f, 4.f );
	EXPECT_EQ( 5.477225575051661f, Length( q1 ) );
}

TEST( Quaternion, QuaternionDot ) 
{
	Quaternion q1( 1.f, 2.f, 3.f, 4.f );
	Quaternion q2( 2.4f, 7.1f, -3.1f, 0.2f );
	EXPECT_FLOAT_EQ( 8.100000381469727f, Dot( q1, q2 ) );
}

TEST( Quaternion, QuaternionIdentity ) 
{
	EXPECT_TRUE( IdentityQuaternion() == Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ) );
}

TEST( Quaternion, QuaternionConjugate ) 
{
	Quaternion q1( 1.0f, -2.0f, 3.0f, 4.0f );
	Quaternion result( -1.0f, 2.0f, -3.0f, 4.0f );

	Quaternion q3 = Conjugate( q1 );
	EXPECT_TRUE( q3 == result );
}

TEST( Quaternion, QuaternionToAxisAngle ) 
{
	Quaternion q1( 0.18257418274879456f, 0.3651483654975891f, 0.5477225184440613f, 0.7302967309951782f );
	Vector3 axis( 0.18257418274879456f, 0.3651483654975891f, 0.5477225184440613f );
	float angle( 1.50408018f );

	auto result = GetAxisAngle( q1 );
	EXPECT_EQ( axis, result.first );
	EXPECT_EQ( angle, result.second );
}

TEST( Quaternion, QuaternionRotationMatrix ) 
{
	Matrix id = IdentityMatrix();

	Quaternion q2 = RotationQuaternion( id );
	EXPECT_QUATERNION_EQ( q2, IdentityQuaternion() );

	Matrix m = RotationXMatrix( 0.5f );
	Quaternion expected( 0.24740396f, 0.00000000f, 0.00000000f, 0.96891242f );

	q2 = RotationQuaternion( m );
	EXPECT_QUATERNION_EQ( expected, q2 );
}

TEST( Quaternion, QuaternionRotationAxis ) 
{
	Vector3 axis( 1.f, 2.f, 3.f );

	Quaternion q2 = RotationQuaternion( axis, 0.0f );
	EXPECT_EQ( q2, IdentityQuaternion() );

	float angle = 0.5f;
	Quaternion expected( 0.066121489f, 0.13224298f, 0.19836447f, 0.96891242f );

	q2 = RotationQuaternion( axis, angle );
	EXPECT_QUATERNION_EQ( expected, q2 );
}

TEST( Quaternion, QuaternionRotationYawPitchRoll ) 
{
	Quaternion q2 = RotationQuaternion( 0.0f, 0.0f, 0.0f );
	EXPECT_EQ( q2, IdentityQuaternion() );

	float yaw = 0.5f;
	float pitch = 0.6f;
	float roll = 0.7f;
	Quaternion expected( 0.350018859f, 0.123841502f, 0.248718783f, 0.894588768f );

	q2 = RotationQuaternion( yaw, pitch, roll );
	EXPECT_QUATERNION_EQ( expected, q2 );
}

TEST( Quaternion, QuaternionNormalize ) 
{
	Quaternion q( 1.f, 2.f, 3.f, 4.f );
	Quaternion expected( 0.18257418f, 0.36514837f, 0.54772258f, 0.73029673f );

	Quaternion q2 = Normalize( q );
	EXPECT_EQ( expected, q2 );
}

TEST( Quaternion, QuaternionInverse ) 
{
	Quaternion id = IdentityQuaternion();

	Quaternion q2 = Inverse( id );
	EXPECT_EQ( IdentityQuaternion(), q2 );

	Quaternion q( 1.f, 2.f, 3.f, 4.f );
	Quaternion expected( -0.033333331f, -0.066666663f, -0.099999994f, 0.13333333f );

	q2 = Inverse( q );
	EXPECT_QUATERNION_EQ( expected, q2 );
}

TEST( Quaternion, QuaternionExp ) 
{
	Quaternion zero( 0.0f, 0.0f, 0.0f, 0.0f );

	Quaternion q2 = Exp( zero );
	EXPECT_EQ( IdentityQuaternion(), q2 );

	Quaternion example( 1.f, 2.f, 3.f, 0.0f );
	Quaternion expected( -0.15092136f, -0.30184272f, -0.45276409f, -0.82529902f );

	q2 = Exp( example );
	EXPECT_QUATERNION_EQ( expected, q2 );
}

TEST( Quaternion, Slerp )
{
	Quaternion a = Normalize( Quaternion( 1.0f, 2.0f, 3.0f, 4.0f ) );
	Quaternion b = Normalize( Quaternion( 0.4f, -0.3f, 0.2f, 0.1f ) );

	Quaternion q1, q2, q3;

	q2 = Slerp( a, b, 0.0f );
	EXPECT_QUATERNION_EQ( q2, a );
	
	q2 = Slerp( a, b, 1.0f );
	EXPECT_QUATERNION_EQ( b, q2 );
	
	Quaternion half( 0.57353938f, -0.11470790f, 0.57353932f, 0.57353932f );

	q2 = Slerp( a, b, 0.5f );
	EXPECT_QUATERNION_EQ( half, q2 );
}
