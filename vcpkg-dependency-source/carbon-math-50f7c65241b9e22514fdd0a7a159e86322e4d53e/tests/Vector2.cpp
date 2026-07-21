// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"

// disable division by 0 warning: we are doing it on purpose
#pragma warning(disable: 4723)


TEST( Vector2Test, Constructors ) 
{
	Vector2 vec1( 1.f, 2.f );
	EXPECT_EQ( 1.f, vec1.x );
	EXPECT_EQ( 2.f, vec1.y );

	Vector2 vec2( vec1 );
	EXPECT_EQ( 1.f, vec2.x );
	EXPECT_EQ( 2.f, vec2.y );
}

TEST( Vector2Test, Additions ) 
{
	Vector2 a( 1.f, 2.f );
	Vector2 b( 4.f, 5.f );

	a += b;
	EXPECT_EQ( 5.f, a.x );
	EXPECT_EQ( 7.f, a.y );

	a += a;
	EXPECT_EQ( 10.f, a.x );
	EXPECT_EQ( 14.f, a.y );

	Vector2 c = a + b;
	EXPECT_EQ( 14.f, c.x );
	EXPECT_EQ( 19.f, c.y );
}

TEST( Vector2Test, Subtractions ) 
{
	Vector2 a( 12.f, 23.f );
	Vector2 b( 4.f, 5.f );

	a -= b;
	EXPECT_EQ( 8.f, a.x );
	EXPECT_EQ( 18.f, a.y );

	Vector2 c = a - b;
	EXPECT_EQ( 4.f, c.x );
	EXPECT_EQ( 13.f, c.y );
	
	Vector2 d = a - a;
	EXPECT_EQ( 0.f, d.x );
	EXPECT_EQ( 0.f, d.y );
}

TEST( Vector2Test, Scaling ) 
{
	Vector2 a( 1.f, 2.f );

	a *= 0.5f;
	EXPECT_EQ( 0.5f, a.x );
	EXPECT_EQ( 1.f, a.y );

	Vector2 b = a * -10.f;
	EXPECT_EQ( -5.f, b.x );
	EXPECT_EQ( -10.f, b.y );

	b /= -2.f;
	EXPECT_EQ( 2.5f, b.x );
	EXPECT_EQ( 5.f, b.y );

	Vector2 c = b / 0.1f;
	EXPECT_EQ( 25.f, c.x );
	EXPECT_EQ( 50.f, c.y );

	b *= 0.f;
	EXPECT_EQ( 0.f, b.x );
	EXPECT_EQ( 0.f, b.y );

	c /= b.x;
	EXPECT_FALSE( CcpIsFinite( c.x ) );
	EXPECT_FALSE( CcpIsFinite( c.y ) );

	Vector2 d( 3.f * a );
	EXPECT_EQ( 1.5f, d.x );
	EXPECT_EQ( 3.f, d.y );
}

TEST( Vector2Test, Signs ) 
{
	Vector2 a( 1.f, 2.f );

	Vector2 b( +a );
	EXPECT_EQ( 1.f, b.x );
	EXPECT_EQ( 2.f, b.y );

	Vector2 c( -a );
	EXPECT_EQ( -1.f, c.x );
	EXPECT_EQ( -2.f, c.y );
}

TEST( Vector2Test, Comparisons ) 
{
	EXPECT_TRUE( Vector2( 1.f, 2.f ) == Vector2( 1.f, 2.f ) );
	EXPECT_FALSE( Vector2( 1.f, 2.f ) == Vector2( 3.f, 2.f ) );
	EXPECT_FALSE( Vector2( 1.f, 2.f ) == Vector2( 1.f, 0.f ) );
	EXPECT_FALSE( Vector2( 1.f, 0.f ) == Vector2( 1.f, 2.f ) );

	EXPECT_FALSE( Vector2( 1.f, 2.f ) != Vector2( 1.f, 2.f ) );
	EXPECT_TRUE( Vector2( 1.f, 2.f ) != Vector2( 3.f, 2.f ) );
	EXPECT_TRUE( Vector2( 1.f, 2.f ) != Vector2( 1.f, 0.f ) );
	EXPECT_TRUE( Vector2( 1.f, 0.f ) != Vector2( 1.f, 2.f ) );
}

TEST( Vector2Test, Length ) 
{
	Vector2 a( 1.f, 2.f );
	EXPECT_EQ( sqrtf( 5.f ), Length( a ) );
}

TEST( Vector2Test, Vec2Normalize ) 
{
	Vector2 a( 1.f, 2.f );
	float l = sqrt( a.x * a.x + a.y * a.y );
	Vector2 b;

	b = Normalize( a );
	EXPECT_EQ( a.x / l, b.x );
	EXPECT_EQ( a.y / l, b.y );
}
