// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"

// disable division by 0 warning: we are doing it on purpose
#pragma warning(disable: 4723)

TEST( Plane, Constructors ) 
{
	Plane p1( 1.f, 2.f, 3.f, 4.f );
	EXPECT_EQ( 1.f, p1.a );
	EXPECT_EQ( 2.f, p1.b );
	EXPECT_EQ( 3.f, p1.c );
	EXPECT_EQ( 4.f, p1.d );

	Plane p2( p1 );
	EXPECT_EQ( 1.f, p2.a );
	EXPECT_EQ( 2.f, p2.b );
	EXPECT_EQ( 3.f, p2.c );
	EXPECT_EQ( 4.f, p2.d );
}

TEST( Plane, Scaling ) 
{
	Plane a( 1.f, 2.f, 3.f, 4.f );

	a *= 0.5f;
	EXPECT_EQ( 0.5f, a.a );
	EXPECT_EQ( 1.f, a.b );
	EXPECT_EQ( 1.5f, a.c );
	EXPECT_EQ( 2.f, a.d );

	Plane b = a * -10.f;
	EXPECT_EQ( -5.f, b.a );
	EXPECT_EQ( -10.f, b.b );
	EXPECT_EQ( -15.f, b.c );
	EXPECT_EQ( -20.f, b.d );

	b /= -2.f;
	EXPECT_EQ( 2.5f, b.a );
	EXPECT_EQ( 5.f, b.b );
	EXPECT_EQ( 7.5f, b.c );
	EXPECT_EQ( 10.f, b.d );

	Plane c = b / 0.1f;
	EXPECT_EQ( 25.f, c.a );
	EXPECT_EQ( 50.f, c.b );
	EXPECT_EQ( 75.f, c.c );
	EXPECT_EQ( 100.f, c.d );

	b *= 0.f;
	EXPECT_EQ( 0.f, b.a );
	EXPECT_EQ( 0.f, b.b );
	EXPECT_EQ( 0.f, b.c );
	EXPECT_EQ( 0.f, b.d );

	c /= b.a;
	EXPECT_FALSE( CcpIsFinite( c.a ) );
	EXPECT_FALSE( CcpIsFinite( c.b ) );
	EXPECT_FALSE( CcpIsFinite( c.c ) );
	EXPECT_FALSE( CcpIsFinite( c.d ) );

	Plane d( 3.f * a );
	EXPECT_EQ( 1.5f, d.a );
	EXPECT_EQ( 3.f, d.b );
	EXPECT_EQ( 4.5f, d.c );
	EXPECT_EQ( 6.f, d.d );
}

TEST( Plane, Signs ) 
{
	Plane a( 1.f, 2.f, 3.f, 4.f );

	Plane b( +a );
	EXPECT_EQ( 1.f, b.a );
	EXPECT_EQ( 2.f, b.b );
	EXPECT_EQ( 3.f, b.c );
	EXPECT_EQ( 4.f, b.d );

	Plane c( -a );
	EXPECT_EQ( -1.f, c.a );
	EXPECT_EQ( -2.f, c.b );
	EXPECT_EQ( -3.f, c.c );
	EXPECT_EQ( -4.f, c.d );
}

TEST( Plane, Comparisons ) 
{
	EXPECT_TRUE( Plane( 1.f, 2.f, 3.f, 4.f ) == Plane( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Plane( 1.f, 2.f, 3.f, 4.f ) == Plane( 3.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Plane( 1.f, 2.f, 3.f, 4.f ) == Plane( 1.f, 0.f, 3.f, 4.f ) );
	EXPECT_FALSE( Plane( 1.f, 2.f, 3.f, 4.f ) == Plane( 1.f, 2.f, 0.f, 4.f ) );
	EXPECT_FALSE( Plane( 1.f, 0.f, 3.f, 4.f ) == Plane( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_FALSE( Plane( 1.f, 0.f, 3.f, 4.f ) == Plane( 1.f, 2.f, 3.f, 5.f ) );

	EXPECT_FALSE( Plane( 1.f, 2.f, 3.f, 4.f ) != Plane( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Plane( 1.f, 2.f, 3.f, 4.f ) != Plane( 3.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Plane( 1.f, 2.f, 3.f, 4.f ) != Plane( 1.f, 0.f, 3.f, 4.f ) );
	EXPECT_TRUE( Plane( 1.f, 2.f, 3.f, 4.f ) != Plane( 1.f, 2.f, 0.f, 4.f ) );
	EXPECT_TRUE( Plane( 1.f, 0.f, 3.f, 4.f ) != Plane( 1.f, 2.f, 3.f, 4.f ) );
	EXPECT_TRUE( Plane( 1.f, 0.f, 3.f, 4.f ) != Plane( 1.f, 2.f, 3.f, 5.f ) );
}

TEST( Plane, PlaneDotCoord ) 
{
	Plane a( 1.f, 2.f, 3.f, 4.f );
	Vector3 b( 4.f, 5.f, 6.f );
	EXPECT_EQ( 36.f, DotCoord( a, b ) );
}

TEST( Plane, PlaneDotNormal ) 
{
	Plane a( 1.f, 2.f, 3.f, 4.f );
	Vector3 b( 4.f, 5.f, 6.f );
	EXPECT_EQ( 32.f, DotNormal( a, b ) );
}

TEST( Plane, PlaneNormalize ) 
{
	Plane a( 1.f, 2.f, 3.f, 4.f );
	float l = sqrt( a.a * a.a + a.b * a.b + a.c * a.c );
	Plane b;

	b = Normalize( a );
	EXPECT_FLOAT_EQ( a.a / l, b.a );
	EXPECT_FLOAT_EQ( a.b / l, b.b );
	EXPECT_FLOAT_EQ( a.c / l, b.c );
	EXPECT_FLOAT_EQ( a.d / l, b.d );
}

TEST( Plane, PlaneIntersectLine ) 
{
	Plane p1( 1.f, 0.f, 0.f, 0.f );
	Vector3 a11( 0.0f, 2.0f, 0.0f );
	Vector3 a12( 0.0f, 3.0f, 0.0f );

	auto result1 = IntersectLine( p1, a11, a12 );
	EXPECT_FALSE( result1.first );

	Vector3 a21( 1.0f, 4.0f, 0.0f );
	Vector3 a22( 2.0f, 4.0f, 0.0f );
	Vector3 expected( 0.0f, 4.0f, 0.0f );

	auto result2 = IntersectLine( p1, a21, a22 );
	EXPECT_TRUE( result2.first );
	EXPECT_EQ( expected, result2.second );
}
