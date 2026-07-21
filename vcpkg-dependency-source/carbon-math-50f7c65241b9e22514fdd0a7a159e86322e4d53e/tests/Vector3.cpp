// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"

// disable division by 0 warning: we are doing it on purpose
#pragma warning(disable: 4723)

#define EXPECT_VECTOR_EQ( expected, actual )			\
	{														\
		EXPECT_FLOAT_EQ( ( expected ).x, ( actual ).x );	\
		EXPECT_FLOAT_EQ( ( expected ).y, ( actual ).y );	\
		EXPECT_FLOAT_EQ( ( expected ).z, ( actual ).z );	\
	}


TEST( Vector3Test, Constructors )
{
	Vector3 vec1( 1.f, 2.f, 3.f );
	EXPECT_EQ( 1.f, vec1.x );
	EXPECT_EQ( 2.f, vec1.y );
	EXPECT_EQ( 3.f, vec1.z );

	Vector3 vec2( vec1 );
	EXPECT_EQ( 1.f, vec2.x );
	EXPECT_EQ( 2.f, vec2.y );
	EXPECT_EQ( 3.f, vec2.z );
}

TEST( Vector3Test, Additions )
{
	Vector3 a( 1.f, 2.f, 3.f );
	Vector3 b( 4.f, 5.f, 6.f );

	a += b;
	EXPECT_EQ( 5.f, a.x );
	EXPECT_EQ( 7.f, a.y );
	EXPECT_EQ( 9.f, a.z );

	a += a;
	EXPECT_EQ( 10.f, a.x );
	EXPECT_EQ( 14.f, a.y );
	EXPECT_EQ( 18.f, a.z );

	Vector3 c = a + b;
	EXPECT_EQ( 14.f, c.x );
	EXPECT_EQ( 19.f, c.y );
	EXPECT_EQ( 24.f, c.z );
}

TEST( Vector3Test, Subtractions )
{
	Vector3 a( 12.f, 23.f, 4.f );
	Vector3 b( 4.f, 5.f, 6.f );

	a -= b;
	EXPECT_EQ( 8.f, a.x );
	EXPECT_EQ( 18.f, a.y );
	EXPECT_EQ( -2.f, a.z );

	Vector3 c = a - b;
	EXPECT_EQ( 4.f, c.x );
	EXPECT_EQ( 13.f, c.y );
	EXPECT_EQ( -8.f, c.z );

	Vector3 d = a - a;
	EXPECT_EQ( 0.f, d.x );
	EXPECT_EQ( 0.f, d.y );
	EXPECT_EQ( 0.f, d.z );
}

TEST( Vector3Test, Scaling )
{
	Vector3 a( 1.f, 2.f, 3.f );

	a *= 0.5f;
	EXPECT_EQ( 0.5f, a.x );
	EXPECT_EQ( 1.f, a.y );
	EXPECT_EQ( 1.5f, a.z );

	Vector3 b = a * -10.f;
	EXPECT_EQ( -5.f, b.x );
	EXPECT_EQ( -10.f, b.y );
	EXPECT_EQ( -15.f, b.z );

	b /= -2.f;
	EXPECT_EQ( 2.5f, b.x );
	EXPECT_EQ( 5.f, b.y );
	EXPECT_EQ( 7.5f, b.z );

	Vector3 c = b / 0.1f;
	EXPECT_EQ( 25.f, c.x );
	EXPECT_EQ( 50.f, c.y );
	EXPECT_EQ( 75.f, c.z );

	b *= 0.f;
	EXPECT_EQ( 0.f, b.x );
	EXPECT_EQ( 0.f, b.y );
	EXPECT_EQ( 0.f, b.z );

	c /= b.x;
	EXPECT_FALSE( CcpIsFinite( c.x ) );
	EXPECT_FALSE( CcpIsFinite( c.y ) );
	EXPECT_FALSE( CcpIsFinite( c.z ) );

	Vector3 d( 3.f * a );
	EXPECT_EQ( 1.5f, d.x );
	EXPECT_EQ( 3.f, d.y );
	EXPECT_EQ( 4.5f, d.z );
}

TEST( Vector3Test, Signs )
{
	Vector3 a( 1.f, 2.f, 3.f );

	Vector3 b( +a );
	EXPECT_EQ( 1.f, b.x );
	EXPECT_EQ( 2.f, b.y );
	EXPECT_EQ( 3.f, b.z );

	Vector3 c( -a );
	EXPECT_EQ( -1.f, c.x );
	EXPECT_EQ( -2.f, c.y );
	EXPECT_EQ( -3.f, c.z );
}

TEST( Vector3Test, Comparisons )
{
	EXPECT_TRUE( Vector3( 1.f, 2.f, 3.f ) == Vector3( 1.f, 2.f, 3.f ) );
	EXPECT_FALSE( Vector3( 1.f, 2.f, 3.f ) == Vector3( 3.f, 2.f, 3.f ) );
	EXPECT_FALSE( Vector3( 1.f, 2.f, 3.f ) == Vector3( 1.f, 0.f, 3.f ) );
	EXPECT_FALSE( Vector3( 1.f, 2.f, 3.f ) == Vector3( 1.f, 2.f, 0.f ) );
	EXPECT_FALSE( Vector3( 1.f, 0.f, 3.f ) == Vector3( 1.f, 2.f, 3.f ) );

	EXPECT_FALSE( Vector3( 1.f, 2.f, 3.f ) != Vector3( 1.f, 2.f, 3.f ) );
	EXPECT_TRUE( Vector3( 1.f, 2.f, 3.f ) != Vector3( 3.f, 2.f, 3.f ) );
	EXPECT_TRUE( Vector3( 1.f, 2.f, 3.f ) != Vector3( 1.f, 0.f, 3.f ) );
	EXPECT_TRUE( Vector3( 1.f, 2.f, 3.f ) != Vector3( 1.f, 2.f, 0.f ) );
	EXPECT_TRUE( Vector3( 1.f, 0.f, 3.f ) != Vector3( 1.f, 2.f, 3.f ) );
}

TEST( Vector3Test, DotProduct )
{
	Vector3 a( 1.f, 2.f, 3.f );
	Vector3 b( 4.f, 5.f, 6.f );
	EXPECT_EQ( 32.f, Dot( a, b ) );
}

TEST( Vector3Test, Length )
{
	Vector3 a( 1.f, 2.f, 3.f );
	EXPECT_EQ( sqrtf( 14.f ), Length( a ) );
}

TEST( Vector3Test, CrossProduct )
{
	Vector3 a( 1.f, 2.f, 3.f );
	Vector3 b( 2.f, 3.f, 4.f );
	Vector3 c;
	c = Cross( a, b );
	EXPECT_EQ( -1.f, c.x );
	EXPECT_EQ( 2.f, c.y );
	EXPECT_EQ( -1.f, c.z );
}

TEST( Vector3Test, Vec3Minimize )
{
	Vector3 a( 14.f, 2.f, 53.f );
	Vector3 b( 3.f, 32.f, 4.f );

	Vector3 c;
	c = Minimize( a, b );
	EXPECT_EQ( 3.f, c.x );
	EXPECT_EQ( 2.f, c.y );
	EXPECT_EQ( 4.f, c.z );
}

TEST( Vector3Test, Vec3Maximize )
{
	Vector3 a( 14.f, 2.f, 53.f );
	Vector3 b( 3.f, 32.f, 4.f );

	Vector3 c;
	c = Maximize( a, b );
	EXPECT_EQ( 14.f, c.x );
	EXPECT_EQ( 32.f, c.y );
	EXPECT_EQ( 53.f, c.z );
}

TEST( Vector3Test, Vec3Lerp )
{
	Vector3 a( 1.f, 2.f, 3.f );
	Vector3 b( 2.f, 10.f, 14.f );

	Vector3 c;
	c = Lerp( a, b, 0.5f );
	EXPECT_EQ( 1.5f, c.x );
	EXPECT_EQ( 6.0f, c.y );
	EXPECT_EQ( 8.5f, c.z );

	c = Lerp( a, b, 1.f );
	EXPECT_EQ( 2.f, c.x );
	EXPECT_EQ( 10.f, c.y );
	EXPECT_EQ( 14.f, c.z );

}

TEST( Vector3Test, Normalize )
{
	Vector3 a( 1.f, 2.f, 3.f );
	float l = sqrt( a.x * a.x + a.y * a.y + a.z * a.z );
	Vector3 b;
	b = Normalize( a );
	EXPECT_FLOAT_EQ( a.x / l, b.x );
	EXPECT_FLOAT_EQ( a.y / l, b.y );
	EXPECT_FLOAT_EQ( a.z / l, b.z );
}

TEST( Vector3Test, Vec3Hermite )
{
	Vector3 v0( 1.f, 2.f, 3.f );
	Vector3 v1( 4.f, 5.f, 6.f );
	Vector3 t0( 7.f, 8.f, 9.f );
	Vector3 t1( 10.f, 11.f, 12.f );
	Vector3 c;

	c = Hermite( v0, t0, v1, t1, 0.f );
	EXPECT_TRUE( c == v0 );
	c = Hermite( v0, t0, v1, t1, 1.f );
	EXPECT_TRUE( c == v1 );
}

TEST( Vector3Test, Vec3Transform )
{
	Matrix rot(
		0.7816392183303833f, 0.5501171946525574f, -0.2939578592777252f, 0.0f,
		-0.4829292893409729f, 0.832030177116394f, 0.27295631170272827f, 0.0f,
		0.3947397768497467f, -0.07139250636100769f, 0.9160150289535522f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f );
	Vector4 result( 3.93171716f, 0.671879411f, 4.90817451f, 1.0f );

	Vector3 v( 1.f, -2.f, 3.f );
	Vector4 r1;
	Vector4 r2 = Transform( v, rot );
	EXPECT_VECTOR_EQ( result, r2 );
}

TEST( Vector3Test, Vec3TransformCoord )
{
	Matrix rot(
		0.7816392183303833f, 0.5501171946525574f, -0.2939578592777252f, 0.0f,
		-0.4829292893409729f, 0.832030177116394f, 0.27295631170272827f, 0.0f,
		0.3947397768497467f, -0.07139250636100769f, 0.9160150289535522f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f );
	Vector3 result( 3.93171716f, 0.671879411f, 4.90817451f );

	Vector3 v( 1.f, -2.f, 3.f );

	Vector3 r2 = TransformCoord( v, rot );
	EXPECT_VECTOR_EQ( result, r2 );
}

TEST( Vector3Test, Vec3TransformNormal )
{
	Matrix rot(
		0.7816392183303833f, 0.5501171946525574f, -0.2939578592777252f, 0.0f,
		-0.4829292893409729f, 0.832030177116394f, 0.27295631170272827f, 0.0f,
		0.3947397768497467f, -0.07139250636100769f, 0.9160150289535522f, 0.0f,
		1.0f, 2.0f, 3.0f, 1.0f );
	Vector3 result( 2.93171716f, -1.32812059f, 1.90817451f );

	Vector3 v( 1.f, -2.f, 3.f );

	Vector3 r2 = TransformNormal( v, rot );
	EXPECT_VECTOR_EQ( result, r2 );
}

TEST( Vector3Test, SphereBoundProbe )
{
	float radius;
	Vector3 center, rayDirection, rayPosition;

	radius = sqrt(77.0f);
	center.x = 1.0f; center.y = 2.0f; center.z = 3.0f;
	rayDirection.x = 2.0f; rayDirection.y = -4.0f; rayDirection.z = 2.0f;
	rayPosition.x = 5.0f; rayPosition.y = 5.0f; rayPosition.z = 9.0f;
	EXPECT_TRUE( SphereBoundProbe( center, radius, rayPosition, rayDirection ) );

	rayPosition.x = 45.0f; rayPosition.y = -75.0f; rayPosition.z = 49.0f;
	EXPECT_FALSE( SphereBoundProbe( center, radius, rayPosition, rayDirection ) );

	rayPosition.x = 5.0f; rayPosition.y = 7.0f; rayPosition.z = 9.0f;
	EXPECT_FALSE( SphereBoundProbe( center, radius, rayPosition, rayDirection ) );

	rayPosition.x = 5.0f; rayPosition.y = 11.0f; rayPosition.z = 9.0f;
	EXPECT_FALSE( SphereBoundProbe( center, radius, rayPosition, rayDirection ) );
}

TEST( Vector3Test, ComputeBoundingSphere )
{
	struct TestStream
	{
		Vector3 point;
		uint16_t padding;
	};

	TestStream stream[2];
	stream[0].point = Vector3( 1.0f, 2.0f, 3.0f );
	stream[0].padding = 1;
	stream[1].point = Vector3( 3.0f, 2.0f, 4.0f );
	stream[1].padding = 2;

	Vector3 center1;
	float radius1;
	Vector3 center2;

	ComputeBoundingSphere( &stream[0].point, 0, sizeof( TestStream ), center1, radius1 );
	EXPECT_EQ( 0.0f, radius1 );
	EXPECT_TRUE( center1 == Vector3( 0.0f, 0.0f, 0.0f ) );

	ComputeBoundingSphere( &stream[0].point, 1, sizeof( TestStream ), center1, radius1 );
	EXPECT_EQ( 0.0f, radius1 );
	EXPECT_TRUE( center1 == stream[0].point );

	ComputeBoundingSphere( &stream[0].point, 2, sizeof( TestStream ), center1, radius1 );
	EXPECT_EQ( 1.118034f, radius1 );
	EXPECT_TRUE( center1 == Vector3( 2.0f, 2.0f, 3.5f ) );
}
