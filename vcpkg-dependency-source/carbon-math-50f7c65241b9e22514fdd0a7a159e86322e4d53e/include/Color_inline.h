// Copyright (c) 2026 CCP Games

#include "Vector4.h"

// --------------------------------------------------------------------------------------
inline constexpr Color::Color():
	r( 0.0f ),
	g( 0.0f ),
	b( 0.0f ),
	a( 0.0f )
{
}

// --------------------------------------------------------------------------------------
inline Color::Color( uint32_t dw )
{
	const float f = 1.0f / 255.0f;
	r = f * (float)(uint8_t)( dw >> 16 );
	g = f * (float)(uint8_t)( dw >> 8 );
	b = f * (float)(uint8_t)( dw >> 0 );
	a = f * (float)(uint8_t)( dw >> 24 );
}

// --------------------------------------------------------------------------------------
inline Color::Color( const Vector4& other ) :
	r( other.x ),
	g( other.y ),
	b( other.z ),
	a( other.w )
{
}

// --------------------------------------------------------------------------------------
inline constexpr Color::Color( float fr, float fg, float fb, float fa ) :
	r( fr ),
	g( fg ),
	b( fb ),
	a( fa )
{
}

// --------------------------------------------------------------------------------------
inline Color::operator uint32_t() const
{
	uint32_t dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (uint32_t)( r * 255.0f + 0.5f );
	uint32_t dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (uint32_t)( g * 255.0f + 0.5f );
	uint32_t dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (uint32_t)( b * 255.0f + 0.5f );
	uint32_t dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (uint32_t)( a * 255.0f + 0.5f );

	return ( dwA << 24 ) | ( dwR << 16 ) | ( dwG << 8 ) | dwB;
}

// --------------------------------------------------------------------------------------
inline Color::operator Vector4() const
{
	return Vector4( r, g, b, a );
}

// --------------------------------------------------------------------------------------
inline Color& Color::operator += ( const Color& color )
{
	r += color.r;
	g += color.g;
	b += color.b;
	a += color.a;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Color& Color::operator -= ( const Color& color )
{
	r -= color.r;
	g -= color.g;
	b -= color.b;
	a -= color.a;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Color& Color::operator *= ( float scale )
{
	r *= scale;
	g *= scale;
	b *= scale;
	a *= scale;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Color& Color::operator /= ( float div )
{
	float scale = 1.0f / div;
	r *= scale;
	g *= scale;
	b *= scale;
	a *= scale;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Color Color::operator + () const
{
	return *this;
}

// --------------------------------------------------------------------------------------
inline Color Color::operator - () const
{
	return Color( -r, -g, -b, -a );
}

// --------------------------------------------------------------------------------------
inline Color Color::operator + ( const Color& other ) const
{
	return Color( r + other.r, g + other.g, b + other.b, a + other.a );
}

// --------------------------------------------------------------------------------------
inline Color Color::operator - ( const Color& other ) const
{
	return Color( r - other.r, g - other.g, b - other.b, a - other.a );
}

// --------------------------------------------------------------------------------------
inline Color Color::operator * ( float scale ) const
{
	return Color( r * scale, g * scale, b * scale, a * scale );
}

// --------------------------------------------------------------------------------------
inline Color Color::operator / ( float div ) const
{
	return Color( *this ) /= div;
}

// --------------------------------------------------------------------------------------
inline bool Color::operator == ( const Color& other ) const
{
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

// --------------------------------------------------------------------------------------
inline bool Color::operator != ( const Color& other ) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

// --------------------------------------------------------------------------------------
inline Color operator * ( float scale, const Color& color )
{
	return Color( color.r * scale, color.g * scale, color.b * scale, color.a * scale );
}

// --------------------------------------------------------------------------------------
inline Color Lerp( const Color& v1, const Color& v2, float s )
{
	return v1 + ( v2 - v1 ) * s;
}

// --------------------------------------------------------------------------------------
inline Color Saturate( const Color& v, float saturation )
{
	if( saturation == 1.f )
	{
		return v;
	}

	// intensity (the magic numbers are values based on how strongly our eyes perceive each color)
	float i = ( v.r * 0.299f ) + ( v.g * 0.587f ) + ( v.b * 0.114f );

	return Lerp( Color( i, i, i, v.a ), v, std::max( 0.0f, saturation ) );
}