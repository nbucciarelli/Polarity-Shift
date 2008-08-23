
#include "datatypes.h"
#include <cstring>

/*

Datatypes source:

Constructors for structs/unions to ensure default values of zero automatically.

*/

#pragma region constructors
vector3::vector3()
{
	memset(&e, 0, sizeof(float) * 3);
}
vector3::vector3(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

quaternion::quaternion()
{
	memset(&e, 0, sizeof(float) * 4);
}

matrix::matrix() //Default to identity matrix
{
	memset(&e, 0, sizeof(float) * 16);

	for(int c = 0; c < 16; c+=5)
		e[c] = 1;
}

matrix::matrix(const matrix& obj)
{
	memcpy(this, &obj, sizeof(matrix));
}

polygon::polygon() : vertexCount(0), vertecies(NULL), maxRadius(0)
{}

#pragma endregion

#pragma region vec3 operators
vector3 vector3::operator+(const vector3& obj) const
{
	return vector3(*this) += obj;
}

vector3 vector3::operator-(const vector3& obj) const
{
	return vector3(*this) -= obj;
}

const vector3& vector3::operator+=(const vector3& obj)
{
	for(int c = 0; c < 3; c++)
		e[c] += obj.e[c];

	return *this;
}

const vector3& vector3::operator-=(const vector3& obj)
{
	for(int c = 0; c < 3; c++)
		e[c] -= obj.e[c];

	return *this;
}

float vector3::operator*(const vector3& obj) const
{
	float dot = 0;

	for(int c = 0; c < 4; c++)
		dot += e[c] * obj.e[c];

	return dot;
}

float vector3::dot2D(const vector3& obj) const
{
	return x * obj.x + y * obj.y;
}

float vector3::length() const
{
	float len = 0;
	for(int c = 0; c < 3; c++)
		len = e[c] * e[c];

	return len;
}

void vector3::normalize()
{
	float len = length();

	for(int c = 0; c < 3; c++)
		e[c] /= len;
}

const vector3& vector3::operator*=(const float& dat)
{
	for(int c = 0; c < 3; c++)
		e[c] *= dat;

	return *this;
}

vector3 vector3::operator*(const float& dat) const
{
	return vector3(*this) *= dat;
}
#pragma endregion

#pragma region matrix operators
const matrix& matrix::operator*=(const matrix& obj)
{
	matrix sol = *this;

	for(int b = 0; b < 4; b++)
		for(int c = 0; c < 4; c++)
			for(int d = 0; d < 4; d++)
				sol.m[b][c] += m[b][d] * obj.m[d][c];

	return *this = sol;
}

const matrix& matrix::operator+=(const matrix& obj)
{
	for(int c = 0; c < 16; c++)
		e[c] += obj.e[c];

	return *this;
}

const matrix& matrix::operator-=(const matrix& obj)
{
	for(int c = 0; c < 16; c++)
		e[c] -= obj.e[c];

	return *this;
}

matrix matrix::operator*(const matrix& obj)
{
	return matrix(*this) *= obj;
}

matrix matrix::operator+(const matrix& obj)
{
	return matrix(*this) -= obj;
}

matrix matrix::operator-(const matrix& obj)
{
	return matrix(*this) -= obj;
}

#pragma endregion

#pragma region texture
textureData::textureData() : ref(0), texture(NULL), width(0), height(0)
{ filename[0] = '\0'; }
#pragma endregion