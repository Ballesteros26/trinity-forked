// Copyright (c) 2026 CCP Games


// ----------------------------------------------------------------------------------
inline constexpr Vector2::Vector2() :
	x( 0.0f ),
	y( 0.0f )
{
}

// ----------------------------------------------------------------------------------
inline constexpr Vector2::Vector2( float x_, float y_ ) :
	x( x_ ),
	y( y_ )
{
}

// ----------------------------------------------------------------------------------
inline Vector2::Vector2( const XMVECTOR& other )
{
	XMStoreFloat2( reinterpret_cast<XMFLOAT2*>( this ), other );
}

// ----------------------------------------------------------------------------------
inline Vector2::operator XMVECTOR() const
{
	return XMLoadFloat2( reinterpret_cast<const XMFLOAT2*>( this ) );
}

// ----------------------------------------------------------------------------------
inline float Vector2::operator[]( int32_t index ) const
{
	return ( &x )[index]; // cppcheck-suppress objectIndex
}

// ----------------------------------------------------------------------------------
inline float& Vector2::operator[]( int32_t index )
{
	return ( &x )[index]; // cppcheck-suppress objectIndex
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator+=( const Vector2& other )
{
	x += other.x;
	y += other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator-=( const Vector2& other )
{
	x -= other.x;
	y -= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator*=( const Vector2& other )
{
	x *= other.x;
	y *= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator/=( const Vector2& other )
{
	x /= other.x;
	y /= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator*=( float f )
{
	x *= f;
	y *= f;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2 Vector2::operator+() const
{
	return Vector2( *this );
}

// ----------------------------------------------------------------------------------
inline Vector2 Vector2::operator-() const
{
	return Vector2( -x, -y );
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator+( const Vector2& other ) const
{
	return Vector2( *this ) += other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator-( const Vector2& other ) const
{
	return Vector2( *this ) -= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator*( const Vector2& other ) const
{
	return Vector2( *this ) *= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator/( const Vector2& other ) const
{
	return Vector2( *this ) /= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator*( float f ) const
{
	return Vector2( *this ) *= f;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator/( float f ) const
{
	return Vector2( *this ) /= f;
}

// ----------------------------------------------------------------------------------
inline bool Vector2::operator==( const Vector2& other ) const
{
	return ( x == other.x && y == other.y );
}

// ----------------------------------------------------------------------------------
inline bool Vector2::operator!=( const Vector2& other ) const
{
	return ( x != other.x || y != other.y );
}

// ----------------------------------------------------------------------------------
inline const Vector2 operator*( float f, const Vector2& other )
{
	return Vector2( other ) *= f;
}

// ----------------------------------------------------------------------------------
inline float Length( const Vector2& v )
{
	return std::sqrt( v.x * v.x + v.y * v.y );
}

// ----------------------------------------------------------------------------------
inline Vector2 Normalize( const Vector2& v )
{
	auto length = Length( v );
	if( length )
	{
		length = 1 / length;
	}
	return Vector2( v.x * length, v.y * length );
}