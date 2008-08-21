#include "datatypes.h"
#include "physics.h"
#include <cmath>

void calc::matrixRotationX(matrix& out, float rad)
{
	float sine = sin(rad);
	float cosine = cos(rad);

	out = matrix();

	out.m[2][2] = out.m[1][1] = cosine;
	out.m[1][2] = -sine;
	out.m[2][3] = sine;
}

void calc::matrixRotationY(matrix& out, float rad)
{
	float sine = sin(rad);
	float cosine = cos(rad);

	out = matrix();

	out.m[0][0] = out.m[2][2] = cosine;
	out.m[0][2] = sine;
	out.m[2][0] = -sine;
}

void calc::matrixRotationZ(matrix& out, float rad)
{
	float sine = sin(rad);
	float cosine = cos(rad);

	out = matrix();

	out.m[0][0] = out.m[1][1] = cosine;
	out.m[0][1] = -sine;
	out.m[1][0] = sine;
}

void calc::matrixRotation(matrix& out, const vector3& angles)
{
	out = matrix();

	//Worked out on paper...This saves a *huge* pile of multiplications.
	float sx = sin(angles.x);
	float cx = cos(angles.x);
	float sy = sin(angles.y);
	float cy = cos(angles.y);
	float sz = sin(angles.z);
	float cz = cos(angles.z);

	out.m[0][0] = cz * cy;
	out.m[0][1] = sz * cx;
	out.m[0][2] = cz * sy * sx + sz * cy * sx;
	
	out.m[1][0] = -sz * cy - cz * sy;
	out.m[1][1] = cz * cx;
	out.m[1][2] = -sz * sy * sx + cz * cy * sx;

	out.m[2][1] = sx;
	out.m[2][2] = cx;


}

void calc::matrixTransform(matrix& out, const vector3& pos, vector3& scale)
{
	out = matrix();

	out.m[0][0] = scale.x;
	out.m[1][1] = scale.y;
	out.m[2][2] = scale.z;

	for(int c = 0; c < 3; c++)
		out.m[3][c] = pos.e[c];
}

void calc::matrixTranslate(matrix& out, const vector3& pos)
{
	out = matrix();

	for(int c = 0; c < 3; c++)
		out.m[3][c] = pos.e[c];
}

void calc::matrixScale(matrix& out, const vector3& scale)
{
	out = matrix();
	out.m[0][0] = scale.x;
	out.m[1][1] = scale.y;
	out.m[2][2] = scale.z;
}

bool calc::isZero(float val, float epsilon)
{
	if(fabs(val) < epsilon)
		return true;
	else
		return false;
}

bool calc::sphereOverlap(const vector3& pt1, float radius1, const vector3& pt2, float radius2)
{
	if((pt2 - pt1).length() < radius1 + radius2)
		return true;
	else
		return false;
}