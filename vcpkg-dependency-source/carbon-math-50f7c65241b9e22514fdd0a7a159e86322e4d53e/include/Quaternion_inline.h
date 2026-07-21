// Copyright (c) 2026 CCP Games

#include "Vector4.h"
#include "Matrix.h"

// --------------------------------------------------------------------------------------
inline constexpr Quaternion::Quaternion() :
	x( 0.0f ),
	y( 0.0f ),
	z( 0.0f ),
	w( 1.0f )
{
}

// --------------------------------------------------------------------------------------
inline constexpr Quaternion::Quaternion( float x_, float y_, float z_, float w_ ) :
	x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion( const XMVECTOR& other )
{
	XMStoreFloat4( reinterpret_cast<XMFLOAT4*>( this ), other );
}

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion( const Vector4& other ) :
	x( other.x ),
	y( other.y ),
	z( other.z ),
	w( other.w )
{
}

// --------------------------------------------------------------------------------------
inline Quaternion::operator XMVECTOR() const
{
	return XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>( this ) );
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator+=( const Quaternion& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator-=( const Quaternion& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator*=( const Quaternion& other )
{
	*this = *this * other;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator*=( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	z *= fDiv;
	w *= fDiv;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion Quaternion::operator+() const
{
	return Quaternion( *this );
}

// --------------------------------------------------------------------------------------
inline Quaternion Quaternion::operator-() const
{
	return Quaternion( -x, -y, -z, -w );
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator+( const Quaternion& other ) const
{
	return Quaternion( *this ) += other;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator-( const Quaternion& other ) const
{
	return Quaternion( *this ) -= other;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator*( const Quaternion& other ) const
{
	Quaternion result;
	result.x = other.w * x + other.x * w + other.y * z - other.z * y;
	result.y = other.w * y - other.x * z + other.y * w + other.z * x;
	result.z = other.w * z + other.x * y - other.y * x + other.z * w;
	result.w = other.w * w - other.x * x - other.y * y - other.z * z;
	return result;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator*( float f ) const
{
	return Quaternion( *this ) *= f;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator/( float f ) const
{
	return Quaternion( *this ) /= f;
}

// --------------------------------------------------------------------------------------
inline bool Quaternion::operator==( const Quaternion& other ) const
{
	return ( x == other.x && y == other.y && z == other.z && w == other.w );
}

// --------------------------------------------------------------------------------------
inline bool Quaternion::operator!=( const Quaternion& other ) const
{
	return ( x != other.x || y != other.y || z != other.z || w != other.w );
}

// --------------------------------------------------------------------------------------
inline const Quaternion operator*( float f, const Quaternion& other )
{
	return Quaternion( other ) *= f;
}

// --------------------------------------------------------------------------------------
inline Quaternion IdentityQuaternion()
{
	return Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
}

// --------------------------------------------------------------------------------------
inline float LengthSq( const Quaternion& q )
{
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

// --------------------------------------------------------------------------------------
inline float Length( const Quaternion& q )
{
	return sqrt( LengthSq( q ) );
}

// --------------------------------------------------------------------------------------
inline Quaternion Normalize( const Quaternion& q )
{
	float l = 1.0f / Length( q );
	return q * l;
}

// --------------------------------------------------------------------------------------
inline Quaternion Inverse( const Quaternion& q )
{
	float l = 1.0f / LengthSq( q );
	return Quaternion( -q.x * l, -q.y * l, -q.z * l, q.w * l );
}

// --------------------------------------------------------------------------------------
inline Quaternion Conjugate( const Quaternion& q )
{
	Quaternion out;
	out.x = -q.x;
	out.y = -q.y;
	out.z = -q.z;
	out.w = q.w;
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion Exp( const Quaternion& q )
{
	float norm = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
	Quaternion out;
	if( norm )
	{
		out.x = sin( norm ) * q.x / norm;
		out.y = sin( norm ) * q.y / norm;
		out.z = sin( norm ) * q.z / norm;
		out.w = cos( norm );
	}
	else
	{
		out.x = 0.0f;
		out.y = 0.0f;
		out.z = 0.0f;
		out.w = 1.0f;
	}
	return out;


}

// --------------------------------------------------------------------------------------
inline float Dot( const Quaternion& q1, const Quaternion& q2 )
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

// --------------------------------------------------------------------------------------
inline Quaternion RotationQuaternion( const Vector3& axis, float angle )
{
	Vector3 temp = Normalize( axis );
	Quaternion out;
	out.x = sin( angle / 2.0f ) * temp.x;
	out.y = sin( angle / 2.0f ) * temp.y;
	out.z = sin( angle / 2.0f ) * temp.z;
	out.w = cos( angle / 2.0f );
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion Slerp( const Quaternion& q1, const Quaternion& q2, float t )
{
	float epsilon = 1.0f;
	float temp = 1.0f - t;
	float u = t;
	float dot = Dot( q1, q2 );
	if( dot < 0.0f )
	{
		epsilon = -1.0f;
		dot = -dot;
	}
	if( 1.0f - dot > 0.001f )
	{
		float theta = acos( dot );
		temp = sin( theta * temp ) / sin( theta );
		u = sin( theta * u ) / sin( theta );
	}
	Quaternion out;
	out.x = temp * q1.x + epsilon * u * q2.x;
	out.y = temp * q1.y + epsilon * u * q2.y;
	out.z = temp * q1.z + epsilon * u * q2.z;
	out.w = temp * q1.w + epsilon * u * q2.w;
	return out;
}

// --------------------------------------------------------------------------------------
inline std::pair<Vector3, float> GetAxisAngle( const Quaternion& q )
{
	return std::make_pair( Vector3( q.x, q.y, q.z ), acos( q.w ) * 2 );
}
