//////////////////////////////////////////////////////////////////////////
//	File	:	"SGD_Math.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	To do lots of math for us.
///////////////////////////////////////////////////////////////////////////
#pragma once

const float SGD_PI = 3.141592653589732f;

////	Define point	-	used for positions
//struct tPoint2D
//{
//	float fX;
//	float fY;
//};

//	Define vector	-	used for movement or calculations
struct tVector2D
{
	float fX;
	float fY;

	tVector2D operator+(const tVector2D& v)
	{
		tVector2D r = { fX + v.fX, fY + v.fY };
		return r;
	}

	tVector2D operator-(const tVector2D& v)
	{
		tVector2D r = { fX - v.fX, fY - v.fY };
		return r;
	}

	tVector2D operator*(const float f)
	{
		tVector2D r = { fX * f, fY * f };
		return r;
	}

	tVector2D operator/(const float f )
	{
		tVector2D r = { fX / f, fY / f };
		return r;
	}
};

//	Find the length of a vector (magnitude)
float Vector2DLength(tVector2D vec);

// Find the dot product
float DotProduct(tVector2D v1, tVector2D v2);

//	Normalize a vector
tVector2D Vector2DNormalize(tVector2D vec);

//	Rotate a vector
/*
	X, Y	=	[X]
				[Y]
				[1]

	[	cos&	sin&	0	]		[X]		[X1]
	[  -sin&	cos&	0	]	*	[Y]	=	[Y1]
	[	0		0		1	]		[1]		[1]
*/
tVector2D Vector2DRotate(tVector2D vec, float fRadians);

//	Find the angle between 2 vectors
/*
	a . b = (||a|| ||b||) cos @

	cos @ = (a . b) / (||a|| ||b||)
	
	@ = arccos (a.b)  
*/
float AngleBetweenVectors(tVector2D v1, tVector2D v2);

//	Linear Interpolation
//	v = a + ((b - a) * t)
float Lerp(float fStart, float fEnd, float fPercent);

//	Tests if two floats are approximately equal given a margin of error
bool FloatEquals(float fResult, float fExpected, float fEpsilon = 0.001f);

//	Determines which way to turn (counter clockwise (+) or clockwise (-))
float Steering(tVector2D vOrientation, tVector2D vToTarget);