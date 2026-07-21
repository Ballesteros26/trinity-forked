// Copyright (c) 2026 CCP Games

#include "Requirements.h"
#include "Quaternion.h"

// --------------------------------------------------------------------------------------
Quaternion RotationQuaternion( const Matrix& m )
{
	Quaternion out;
	int i, maxi;
	float maxdiag, S, trace;

	trace = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;
	if( trace > 1.0f )
	{
		out.x = ( m.m[1][2] - m.m[2][1] ) / ( 2.0f * sqrt( trace ) );
		out.y = ( m.m[2][0] - m.m[0][2] ) / ( 2.0f * sqrt( trace ) );
		out.z = ( m.m[0][1] - m.m[1][0] ) / ( 2.0f * sqrt( trace ) );
		out.w = sqrt( trace ) / 2.0f;
		return out;
	}
	maxi = 0;
	maxdiag = m.m[0][0];
	for( i = 1; i<3; i++ )
	{
		if( m.m[i][i] > maxdiag )
		{
			maxi = i;
			maxdiag = m.m[i][i];
		}
	}
	switch( maxi )
	{
	case 0:
		S = 2.0f * sqrt( 1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2] );
		out.x = 0.25f * S;
		out.y = ( m.m[0][1] + m.m[1][0] ) / S;
		out.z = ( m.m[0][2] + m.m[2][0] ) / S;
		out.w = ( m.m[1][2] - m.m[2][1] ) / S;
		break;
	case 1:
		S = 2.0f * sqrt( 1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2] );
		out.x = ( m.m[0][1] + m.m[1][0] ) / S;
		out.y = 0.25f * S;
		out.z = ( m.m[1][2] + m.m[2][1] ) / S;
		out.w = ( m.m[2][0] - m.m[0][2] ) / S;
		break;
	case 2:
		S = 2.0f * sqrt( 1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1] );
		out.x = ( m.m[0][2] + m.m[2][0] ) / S;
		out.y = ( m.m[1][2] + m.m[2][1] ) / S;
		out.z = 0.25f * S;
		out.w = ( m.m[0][1] - m.m[1][0] ) / S;
		break;
	}
	return out;
}

// --------------------------------------------------------------------------------------
Quaternion RotationQuaternion( float yaw, float pitch, float roll )
{
#if __APPLE__
	// We need a special case for apple because of a bug that makes XMVectorRound not work
	// See info here: https://githubhot.com/repo/microsoft/DirectXMath/issues/127
	float sinYaw = sin( yaw / 2.0f );
	float cosYaw = cos( yaw / 2.0f );
	float sinPitch = sin( pitch / 2.0f );
	float cosPitch = cos( pitch / 2.0f );
	float sinRoll = sin( roll / 2.0f );
	float cosRoll = cos( roll / 2.0f );

	Quaternion out;
	out.x = sinYaw * cosPitch * sinRoll + cosYaw * sinPitch * cosRoll;
	out.y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
	out.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
	out.w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
	return out;
#else
    return Quaternion(XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
#endif
}
