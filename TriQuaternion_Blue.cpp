#include "StdAfx.h"
#include "include/TriQuaternion.h"
#include "Include/ITriVector.h"
#include "Include/ITriMatrix.h"

#if BLUE_WITH_PYTHON
BLUE_DEFINE( TriQuaternion );

const Be::ClassInfo* TriQuaternion::ExposeToBlue()
{
	EXPOSURE_BEGIN(TriQuaternion, TRIQUATERNION_Description)
		MAP_INTERFACE(ITriQuaternion)
		MAP_INTERFACE(IPythonMethods)			

		////////////////////////////////////////////////////////////////////////////
		//               x
		MAP_ATTRIBUTE
		( 
			"x", 
			x, 
			"", 
			Be::READWRITE | Be::PERSIST 
		)

		////////////////////////////////////////////////////////////////////////////
		//               y
		MAP_ATTRIBUTE
		( 
			"y", 
			y, 
			"", 
			Be::READWRITE | Be::PERSIST 
		)

		////////////////////////////////////////////////////////////////////////////
		//               z
		MAP_ATTRIBUTE
		( 
			"z", 
			z, 
			"", 
			Be::READWRITE | Be::PERSIST 
		)

		////////////////////////////////////////////////////////////////////////////
		//               w
		MAP_ATTRIBUTE
		( 
			"w", 
			w, 
			"", 
			Be::READWRITE | Be::PERSIST 
		)


		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS( 
			"__init__",
			Py__init__, 
			4,
			"Constructor arguments\n"
			":param x: \n"
			":param y: \n"
			":param z: \n"
			":param w: \n"
			)

		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AND_WRAP( 
			"SetXYZW",
			SetXYZW, 
			"sets the quaternion\n"
			":param x: \n"
			":param y: \n"
			":param z: \n"
			":param w: \n"
			)

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Conjugate",
			PyConjugate, 
			"conjugates the quaternion.")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Dot",
			PyDot, 
			"Accepts one argument, a quaternion. The result is a Dot product of this quaternion"
			"and argument 1\n"
			":param other: \n"
			)

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Exp",
			PyExp, 
			"exponents the quaternion.")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Identity",
			PyIdentity, 
			"Makes the quaternion an identity quaternion")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Inverse",
			PyInverse, 
			"inverses the quaternion")
		
		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"IsIdentity",
			PyIsIdentity, 
			"Determines if a quaternion is an identity quaternion")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Length",
			PyLength, 
			"Returns the length of a quaternion")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"LengthSq",
			PyLengthSq, 
			"Returns the square of the length of a quaternion.")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Ln",
			PyLn, 
			"Sets this quaternion to its natural algoritm")

		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Multiply",
			PyMultiply, 
			"Multiplies a quaternion with this quaternion\n"
			":param other: \n"
			)
			
		////////////////////////////////////////////////////////////////////////////			
		MAP_METHOD_AND_WRAP( 
			"Normalize",
			PyNormalize, 
			"Normalizes this quaternion" )

		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AND_WRAP( 
			"RotationAxis",
			PyRotationAxis, 
			"Set the rotation of the quaternion\n"
			":param axis: \n"
			":param angle: \n"
			)

		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AND_WRAP( 
			"RotationMatrix",
			PyRotationMatrix, 
			"Set the rotation of the quaternion\n" 
			":param matrix: \n"
			)

		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AND_WRAP( 
			"YawPitchRoll",
			PyYawPitchRoll, 
			"Set the rotation of the quaternion\n"
			":param yaw: \n"
			":param pitch: \n"
			":param roll: \n"
			)

		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AND_WRAP( 
			"Slerp",
			PySlerp, 
			":param other: \n"
			":param t: \n"
			)
			
		////////////////////////////////////////////////////////////////////////////
		MAP_METHOD_AS_METHOD( 
			"ToAxisAngle",
			PyToAxisAngle, 
			":rtype: (TriVector, float)" )


		////////////////////////////////////////////////////////////////////////////
		//               SetIdentity
		MAP_METHOD_AND_WRAP
		(
			"SetIdentity",
			SetIdentity, 
			"Sets quaternion to identity quaternion" 
		)

		////////////////////////////////////////////////////////////////////////////
		//               SetRotationAxis
		MAP_METHOD_AND_WRAP
		( 
			"SetRotationAxis",
			PySetRotationAxis, 
			"Set the rotation of the quaternion\n" 
			":param axis: \n"
			":param angle: \n"
		)

		////////////////////////////////////////////////////////////////////////////
		//               GetRotationAxis
		MAP_METHOD_AS_METHOD
		( 
			"GetRotationAxis",
			PyGetRotationAxis, 
			"Get the rotation of the quaternion\n" 
			":rtype: (TriVector, float)"
		)			

		////////////////////////////////////////////////////////////////////////////
		//               SetYawPitchRoll
		MAP_METHOD_AND_WRAP
		( 
			"SetYawPitchRoll",
			SetYawPitchRoll, 
			"Set the yaw pitch roll of the quaternion\n" 
			":param yaw: \n"
			":param pitch: \n"
			":param roll: \n"
		)			

		////////////////////////////////////////////////////////////////////////////
		//               GetYawPitchRoll
		MAP_METHOD_AND_WRAP
		( 
			"GetYawPitchRoll",
			PyGetYawPitchRoll, 
			"Get the yaw pitch roll of the quaternion\n" 
			":rtype: (float, float, float)"
		)			

		////////////////////////////////////////////////////////////////////////////
		//               IncreaseYawPitchRoll
		MAP_METHOD_AND_WRAP
		( 
			"IncreaseYawPitchRoll",
			IncreaseYawPitchRoll, 
			"Increases the rotation of the quaternion\n" 
			":param yaw: \n"
			":param pitch: \n"
			":param roll: \n"
		)

		////////////////////////////////////////////////////////////////////////////
		//               IncreaseLocalYawPitchRoll
		MAP_METHOD_AND_WRAP
		( 
			"IncreaseLocalYawPitchRoll",
			IncreaseLocalYawPitchRoll, 
			"Increases the rotation of the quaternion," 
			"relative to the current rotation\n" 
			":param yaw: \n"
			":param pitch: \n"
			":param roll: \n"
		)

		////////////////////////////////////////////////////////////////////////////
		//               SetRotationArc
		MAP_METHOD_AND_WRAP
		( 
			"SetRotationArc",
			PySetRotationArc, 
			"Set the rotation of the quaternion from one vector to another\n" 
			":param v1: \n"
			":param v2: \n"
		)			

		////////////////////////////////////////////////////////////////////////////
		//               MultiplyQuaternion
		MAP_METHOD_AND_WRAP
		( 
			"MultiplyQuaternion",
			PyMultiplyQuaternion, 
			"Multiplies a quaternion with this quaternion\n" 
			":param other: \n"
		)
		////////////////////////////////////////////////////////////////////////////
		//               SetSLERP
		MAP_METHOD_AND_WRAP
		( 
			"SetSLERP",
			PySetSLERP, 
			"Sets this quaternion to the spherical linear interpolation between the first one and the second. The t determines how far to interpolate.\n" 
			":param q1: \n"
			":param q2: \n"
			":param t: \n"
		)
		////////////////////////////////////////////////////////////////////////////
		//               Scale
		MAP_METHOD_AND_WRAP
		( 
			"Scale",
			PyScale, 
			"Scales the quaternion\n" 
			":param scale: \n"
		)
		////////////////////////////////////////////////////////////////////////////
		//               SetExp
		MAP_METHOD_AND_WRAP
		( 
			"SetExp",
			PySetExp, 
			"Sets this quaternion as the exponential of 'in'\n" 
			":param other: \n"
		)
		////////////////////////////////////////////////////////////////////////////
		//               SetLn
		MAP_METHOD_AND_WRAP
		( 
			"SetLn",
			PySetLn, 
			"Sets this quaternion as the logarithm of 'in'\n" 
			":param other: \n"
		)

		////////////////////////////////////////////////////////////////////////////
		//               SetPower
		MAP_METHOD_AND_WRAP
		( 
			"Pow",
			PyPow, 
			"Sets this quaternion as this**pow\n" 
			":param other: \n"
		)

	EXPOSURE_END()

}

#endif
