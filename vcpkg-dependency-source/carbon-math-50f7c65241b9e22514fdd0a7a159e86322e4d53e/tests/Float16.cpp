// Copyright (c) 2026 CCP Games

#include "CcpFloat.h"
#include "gtest/gtest.h"
#include "CcpMath.h"
#include <limits>

namespace
{
	uint16_t AsUint( Float_16 v )
	{
		return *reinterpret_cast<uint16_t*>( &v );
	}
}

TEST( Float16, ConstructFromFloat ) 
{
	Float_16 f1( 0.0f );
	EXPECT_EQ( 0, AsUint( f1 ) );

	Float_16 f2( -0.0f );
	EXPECT_EQ( 0x8000, AsUint( f2 ) );

	Float_16 f3( 1.0f );
	EXPECT_EQ( 0x3c00, AsUint( f3 ) );

	Float_16 f4( -1.0f );
	EXPECT_EQ( AsUint( f3 ) | 0x8000, AsUint( f4 ) );
}

TEST( Float16, From32To16To32 ) 
{
	Float_16 f1( 0.0f );
	EXPECT_EQ( 0.0f, float( f1 ) );

	Float_16 f2( -0.0f );
	EXPECT_EQ( -0.0f, float( f2 ) );

	Float_16 f3( 1.0f );
	EXPECT_EQ( 1.0f, float( f3 ) );

	Float_16 f4( -1.0f );
	EXPECT_EQ( -1.0f, float( f4 ) );

	Float_16 f5( std::numeric_limits<float>::quiet_NaN() );
	EXPECT_TRUE( CcpIsNaN( float( f5 ) ) );
}

TEST( Float16, Comparisons ) 
{
	EXPECT_TRUE( Float_16( 0.0f ) == Float_16( 0.0f ) );
	EXPECT_TRUE( Float_16( 1.0f ) == Float_16( 1.0f ) );
	EXPECT_TRUE( Float_16( -1.0f ) == Float_16( -1.0f ) );
	EXPECT_TRUE( Float_16( std::numeric_limits<float>::quiet_NaN() ) == Float_16( std::numeric_limits<float>::quiet_NaN() ) );

	EXPECT_FALSE( Float_16( -0.0f ) == Float_16( 0.0f ) );
	EXPECT_FALSE( Float_16( 1.0f ) == Float_16( 0.0f ) );
	EXPECT_FALSE( Float_16( 0.0f ) == Float_16( 1.0f ) );
	EXPECT_FALSE( Float_16( -1.0f ) == Float_16( 1.0f ) );

	EXPECT_FALSE( Float_16( 0.0f ) != Float_16( 0.0f ) );
	EXPECT_FALSE( Float_16( 1.0f ) != Float_16( 1.0f ) );
	EXPECT_FALSE( Float_16( -1.0f ) != Float_16( -1.0f ) );
	EXPECT_FALSE( Float_16( std::numeric_limits<float>::quiet_NaN() ) != Float_16( std::numeric_limits<float>::quiet_NaN() ) );

	EXPECT_TRUE( Float_16( -0.0f ) != Float_16( 0.0f ) );
	EXPECT_TRUE( Float_16( 1.0f ) != Float_16( 0.0f ) );
	EXPECT_TRUE( Float_16( 0.0f ) != Float_16( 1.0f ) );
	EXPECT_TRUE( Float_16( -1.0f ) != Float_16( 1.0f ) );
}
