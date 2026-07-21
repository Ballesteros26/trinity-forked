// Copyright (c) 2026 CCP Games

#pragma once

#include "Requirements.h"


struct Vector2;
struct Vector3;
struct Vector4;
struct Quaternion;


struct Matrix
{
	constexpr Matrix();
	constexpr Matrix(
		float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44 );
	Matrix( const XMMATRIX& other );

	float& operator()( uint32_t row, uint32_t col );
	float operator()( uint32_t row, uint32_t col ) const;

	operator XMMATRIX() const;

	Matrix& operator+=( const Matrix& other );
	Matrix& operator-=( const Matrix& other );
	Matrix& operator*=( const Matrix& other );
	Matrix& operator*=( float f );
	Matrix& operator/=( float f );

	Matrix operator+() const;
	Matrix operator-() const;

	const Matrix operator+( const Matrix& other ) const;
	const Matrix operator-( const Matrix& other ) const;
	const Matrix operator*( const Matrix& other ) const;
	const Matrix operator*( float f ) const;
	const Matrix operator/( float f ) const;

	bool operator==( const Matrix& other ) const;
	bool operator!=( const Matrix& other ) const;

	const Vector3& GetTranslation() const;
	Vector3& GetTranslation();
	const Vector3& GetX() const;
	Vector3& GetX();
	const Vector3& GetY() const;
	Vector3& GetY();
	const Vector3& GetZ() const;
	Vector3& GetZ();

	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;

		};
		float m[4][4];
	};
};


const Matrix operator*( float f, const Matrix& other );
Vector4 operator*( Vector4 p, const Matrix& m );
Vector4 operator*( const Matrix& m, Vector4 p );

Matrix IdentityMatrix();
Matrix TranslationMatrix( float x, float y, float z );
Matrix TranslationMatrix( const Vector3& translation );
Matrix ScalingMatrix( float sx, float sy, float sz );
Matrix ScalingMatrix( const Vector3& scaling );
Matrix RotationMatrix( const Quaternion& q );
Matrix RotationMatrix( const Vector3& axis, float angle );
Matrix RotationXMatrix( float angle );
Matrix RotationYMatrix( float angle );
Matrix RotationZMatrix( float angle );
Matrix OrthoNormalBasisZ( const Vector3& z );
Matrix OrthographicProjection( float width, float height, float zNear, float zFar );
Matrix TransformationMatrix(
	const Vector3* scalingCenter,
	const Quaternion* scalingRotation,
	const Vector3* scaling,
	const Vector3* rotationCenter,
	const Quaternion *rotation,
	const Vector3* translation );
Matrix TransformationMatrix( const Vector3& scaling, const Quaternion& rotation, const Vector3& translation );
Matrix Transformation2DMatrix(
	const Vector2* scalingCenter,
	float scalingRotation,
	const Vector2* scaling,
	const Vector2* rotationCenter,
	float rotation,
	const Vector2* translation );
Matrix LookAtMatrix( const Vector3& peye, const Vector3& pat, const Vector3& pup );
Matrix PerspectiveFovMatrix( float fovy, float aspect, float zn, float zf );
Matrix PerspectiveOffCenterMatrix( float l, float r, float b, float t, float zn, float zf );
Matrix OrthoMatrix( float w, float h, float zn, float zf );
Matrix OrthoOffCenterMatrix( float l, float r, float b, float t, float zn, float zf );

Matrix Transpose( const Matrix& m );
float Determinant( const Matrix& m );
Matrix Inverse( const Matrix& m );
bool Inverse( Matrix& out, float& det, const Matrix& m );
bool Inverse( Matrix& out, const Matrix& m );

Vector3 TransformCoord( const Vector3& coord, const Matrix& transform );
Vector3 TransformNormal( const Vector3& normal, const Matrix& transform );
Vector4 Transform( const Vector4& point, const Matrix& transform );
Vector4 Transform( const Vector3& point, const Matrix& transform );
Vector4 Transform( const Vector2& v, const Matrix& m );
void TransformCoords( void* coords, size_t count, size_t stride, const Matrix& transform );
void TransformCoords( Vector3* coords, size_t size, const Matrix& transform );
template <size_t Size> void TransformCoords( Vector3( &coords )[Size], const Matrix& transform );
template <typename OutIter, typename InIter> void TransformCoords( OutIter dest, InIter first, InIter last, const Matrix& transform );

void Decompose( Vector3& scale, Quaternion& rotation, Vector3& translation, const Matrix& m );

#include "Matrix_inline.h"
