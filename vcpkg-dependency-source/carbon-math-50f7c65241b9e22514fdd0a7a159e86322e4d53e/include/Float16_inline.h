// Copyright (c) 2026 CCP Games

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

static_assert( sizeof( Float_16 ) == 2, "Float_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector2_16 ) == 2 * 2, "Vector2_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector3_16 ) == 3 * 2, "Vector3_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector4_16 ) == 4 * 2, "Vector4_16 size incorrect, alignment issues?" );


// --------------------------------------------------------------------------------------
inline constexpr Float_16::Float_16() :
	m_value( 0 )
{
}

// --------------------------------------------------------------------------------------
inline Float_16::Float_16( float other ) :
	m_value( Float32To16( other ) )
{
}

// --------------------------------------------------------------------------------------
inline constexpr Float_16::Float_16( uint16_t other ) :
	m_value( other )
{
}

// --------------------------------------------------------------------------------------
inline Float_16::operator float() const
{
	return Float16To32( m_value );
}

// --------------------------------------------------------------------------------------
inline bool Float_16::operator == ( const Float_16& other ) const
{
	return m_value == other.m_value;
}

// --------------------------------------------------------------------------------------
inline bool Float_16::operator != ( const Float_16& other ) const
{
	return m_value != other.m_value;
}

// --------------------------------------------------------------------------------------
inline constexpr Vector2_16::Vector2_16()
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16( float x_, float y_ ) :
	x( x_ ),
	y( y_ )
{
}

// --------------------------------------------------------------------------------------
inline constexpr Vector2_16::Vector2_16( Float_16 x_, Float_16 y_ ) :
	x( x_ ),
	y( y_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16( const Vector2& other ) :
	x( other.x ),
	y( other.y )
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::operator Vector2() const
{
	return Vector2( x, y );
}

// --------------------------------------------------------------------------------------
inline constexpr Vector3_16::Vector3_16()
{
}

// --------------------------------------------------------------------------------------
inline constexpr Vector3_16::Vector3_16( Float_16 x_, Float_16 y_, Float_16 z_ ) :
	x( x_ ),
	y( y_ ),
	z( z_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16( float x_, float y_, float z_ ) :
	x( x_ ),
	y( y_ ),
	z( z_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16( const Vector3& other ) :
	x( other.x ),
	y( other.y ),
	z( other.z )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::operator Vector3() const
{
	return Vector3( x, y, z );
}

// --------------------------------------------------------------------------------------
inline constexpr Vector4_16::Vector4_16()
{
}

// --------------------------------------------------------------------------------------
inline constexpr Vector4_16::Vector4_16( Float_16 x_, Float_16 y_, Float_16 z_, Float_16 w_ ) :
	x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16( float x_, float y_, float z_, float w_ ) :
	x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16( const Vector4& other ) :
	x( other.x ),
	y( other.y ),
	z( other.z ),
	w( other.w )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::operator Vector4() const
{
	return Vector4( x, y, z, w );
}
