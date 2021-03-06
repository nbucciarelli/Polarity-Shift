#pragma once
//Physics functions.

union vector3;
union matrix;
struct polygon;
struct polyCollision;

#define GRAVITY 392.0f/2

class calc
{
public:
	static const float infinity;

	static float angleBetweenVects(vector3& v1, vector3& v2);
	//matrix Rotation functions:  Return a rotation matrix for given radians
	static void matrixRotationX(matrix& out, float rad);
	static void matrixRotationY(matrix& out, float rad);
	static void matrixRotationZ(matrix& out, float rad);

	//rotation matrix combining X,Y,Z, not rotation about an axis
	static void matrixRotation(matrix& out, const vector3& angles);

	//return matrix for given xyz scale
	static void matrixScale(matrix& out, const vector3& scale);

	//translation matrix for given coordinates
	static void matrixTranslate(matrix& out, const vector3& pos);

	//Combination of scaling & transformation
	static void matrixTransform(matrix& out, const vector3& pos, vector3& scale);
	
	//returns true if given value is within zero tolerance
	static bool isZero(float, float epsilon = EPSILON);
	static bool isZero(const vector3&, float epsilon = EPSILON);

	//return true if spheres at given points overlap
	static bool sphereOverlap(const vector3& pt1, float radius1, const vector3& pt2, float radius2);

	//returns true if polygons collide. impactVect,
	//if defined, returns the normal of the edge of
	//poly 1 that registers collision
	static bool polygonCollision(const polygon& poly1, const polygon& poly2, 
		const vector3* velocity, polyCollision* results = NULL);

	//min & max are return variables.
	static void projectPolygonToLine(const polygon& poly, const vector3& line, float& min, float& max);
	static void projectRectToLine(const rect& box, const vector3& line, float& min, float& max);

	//Return the space between each 1d line.  Negative if overlapping
	static float distanceBetweenLines(float min1, float max1, float min2, float max2);

	static bool lineIntersectPoly(const vector3& pt1,
								  const vector3& pt2,
								  const polygon& poly,
								  float* distance = 0);

	static vector3 rotatePointAroundOrigin(const vector3& point, const float rad);

	//If defined, overlapVect will be assigned as the distance overlapped 
	static bool polyIntersectRect(const polygon& poly, const rect& box,
								  vector3* overlapVect = NULL);

	//Basically the same as poly/poly, except with a rect.
	static void rectToPoly(const rect &box, polygon* poly);
};