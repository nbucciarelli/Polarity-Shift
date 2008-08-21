#pragma once
//Physics functions.

union vector3;
union matrix;

class calc
{
public:
	static void matrixRotationX(matrix& out, float rad);
	static void matrixRotationY(matrix& out, float rad);
	static void matrixRotationZ(matrix& out, float rad);
	static void matrixRotation(matrix& out, const vector3& angles);

	static void matrixScale(matrix& out, const vector3& scale);

	static void matrixTranslate(matrix& out, const vector3& pos);
	static void matrixTransform(matrix& out, const vector3& pos, vector3& scale);
	static bool isZero(float, float epsilon = EPSILON);

	static bool sphereOverlap(const vector3& pt1, float radius1, const vector3& pt2, float radius2);
};