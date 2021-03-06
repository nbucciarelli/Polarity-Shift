#pragma once

/*
Datatypes H:

Information for all internal data types, universal constants, (and possibly basic functions)

All Unions/structs are set up to start with zero-values.

*/

#define PI 3.14159f
#define _2PI 6.28318f
#define PIHALF 1.570795f
#define EPSILON 0.001f
#ifndef NULL
#define NULL 0
#endif
#define MAX_PATH 260

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint color;

struct pt;

union vector3 {
	float e[3];

	struct {
		float x,y,z;
	};

	vector3(float _x = 0, float _y = 0, float _z = 0);

	vector3 operator+(const vector3&) const;
	vector3 operator-(const vector3&) const;
	const vector3& operator+=(const vector3&);
	const vector3& operator-=(const vector3&);

	const vector3& operator+=(const pt&);
	const vector3& operator-=(const pt&);
	vector3 operator+(const pt&) const;
	vector3 operator-(const pt&) const;

	vector3 operator*(const float&) const;
	const vector3& operator*=(const float&);

	//Dot Product
	float operator*(const vector3&) const;

	float dot2D(const vector3&) const;
	void normalize();
	vector3 normalized() const;
	float length() const;
	void zeroDrift();
};

union quaternion {
	float e[4];

	struct {
		float q,x,y,z;
	};

	quaternion();
};

union matrix {
	float e[16];

	float m[4][4];

	matrix();
	matrix(const matrix&);

	const matrix& operator*=(const matrix&);
	const matrix& operator+=(const matrix&);
	const matrix& operator-=(const matrix&);
	matrix operator*(const matrix&);
	matrix operator+(const matrix&);
	matrix operator-(const matrix&);
	vector3 operator*(const vector3&);
};

struct rect
{
	long left, top, right, bottom;
};

struct pt
{
	int x,y;

	pt(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

struct textureData
{
	char filename[MAX_PATH];
	int ref;			//Ref count, how many things are using it
	void* texture;		//Texture pointer type varies depending on renderer
	int width;
	int height;

	textureData();
};

struct objectPoint
{
	vector3 coords;
	float mass;

	objectPoint(float x = 0, float y = 0, float z = 0, float m = 0)
		: mass(m), coords(x,y,z) {}
};

struct polygon
{
	int vertexCount;
	objectPoint* vertecies;
	objectPoint center;

	float maxRadius;
	
	polygon();
	~polygon();
};

struct polyCollision
{
	bool overlapped, willCollide;
	vector3 responseVect, overlap;
};