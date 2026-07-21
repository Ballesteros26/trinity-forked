// Copyright (c) 2026 CCP Games

#pragma once

#include "Vector3.h"

namespace CcpMath
{

struct Ray
{
	constexpr Ray() = default;
	constexpr Ray( const Vector3& origin, const Vector3& direction );

	Vector3 origin;
	Vector3 direction;
};

}

#include "Ray_inline.h"