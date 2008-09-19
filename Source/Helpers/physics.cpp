#include "datatypes.h"
#include "physics.h"
#include <cmath>

#include <d3dx9.h>

#pragma comment(lib, "d3dx9.lib")

const float calc::infinity = (float)_HUGE;

//TODO:  RECALCULATE MATRIX EQUATIONS, get rid of d3dx references

#pragma region datatype extra
const matrix& matrix::operator*=(const matrix& obj)
{
	/*matrix sol = *this;

	for(int b = 0; b < 4; b++)
		for(int c = 0; c < 4; c++)
			for(int d = 0; d < 4; d++)
				sol.m[b][c] += m[b][d] * obj.m[d][c];

	return *this = sol;*/

	*((D3DXMATRIX*)this) *= *((D3DXMATRIX*)&obj);

	return *this;
}

void vector3::zeroDrift()
{
	for(int c = 0; c < 3; c++)
		if(calc::isZero(e[c]))
			e[c] = 0;
}
#pragma endregion

#pragma region matrix rotation

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
/*	float sine = sin(rad);
	float cosine = cos(rad);

	out = matrix();

	out.m[0][0] = out.m[2][2] = cosine;
	out.m[0][2] = sine;
	out.m[2][0] = -sine; */

	D3DXMatrixRotationY((D3DXMATRIX*)(&out), rad);
}

void calc::matrixRotationZ(matrix& out, float rad)
{
/*	float sine = sin(rad);
	float cosine = cos(rad);

	out = matrix();

	out.m[0][0] = out.m[1][1] = cosine;
	out.m[0][1] = -sine;
	out.m[1][0] = sine;
	*/
	D3DXMatrixRotationZ((D3DXMATRIX*)(&out), rad);
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
#pragma endregion

#pragma region matrix transforms
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
	/*out = matrix();

	for(int c = 0; c < 3; c++)
		out.m[3][c] = pos.e[c];*/

	D3DXMatrixTranslation((D3DXMATRIX*)&out, pos.x, pos.y, pos.z);
}

void calc::matrixScale(matrix& out, const vector3& scale)
{
/*	out = matrix();
	out.m[0][0] = scale.x;
	out.m[1][1] = scale.y;
	out.m[2][2] = scale.z;
	*/

	D3DXMatrixScaling((D3DXMATRIX*)&out, scale.x, scale.y, scale.z);
}

#pragma endregion

#pragma region general calculations

bool calc::isZero(float val, float epsilon)
{
	if(fabs(val) < epsilon)
		return true;
	else
		return false;
}

bool calc::isZero(const vector3& vec, float epsilon)
{
	for(int c = 0; c < 3; c++)
		if(!isZero(vec.e[c]))
			return false;

	return true;
}

vector3 calc::rotatePointAroundOrigin(const vector3& point, const float rad)
{
	//This is matrix math for a 2D rotation about the origin.
	//Just skipping the matrix.

	if(isZero(rad))
		return point;

	float sine = sin(rad), cosine = cos(rad);

	return vector3(
		point.x * cosine + point.y * -sine,
		point.x * sine + point.y * cosine,
		0
		);;
}
#pragma endregion

#pragma region polygon collision

#pragma region main
bool calc::polygonCollision(const polygon& poly1, const polygon& poly2,
							const vector3* velocity, polyCollision* results)
{
	polyCollision result;
	result.overlapped = result.willCollide = true;

	const polygon* poly[2] = { &poly1, &poly2 };

	vector3 approach = (poly1.center.coords - poly2.center.coords);
	vector3 responseVect;
	float min1, max1, min2, max2;
	min1 = max1 = min2 = max2 = 0;

	float minInterval = (float)_HUGE, minOverlap = (float)_HUGE;

	//code-efficient (read: less typing) way of checking through both polys.
	for(int p = 0; p < 2; p++)
	{
		for(int c = 0; c < poly[p]->vertexCount; c++)
		{
			//get the current edge
			vector3 edgevector;
			if(c)
				edgevector = poly[p]->vertecies[c].coords - poly[p]->vertecies[c-1].coords;
			else
				edgevector = poly[p]->vertecies[c].coords - poly[p]->vertecies[poly[p]->vertexCount - 1].coords;

			//and the normal of the edge
			vector3 normvect = vector3(edgevector.y, -edgevector.x, 0);
			normvect.normalize();

			//Squish polygons to current edge vector
			projectPolygonToLine(poly1, normvect, min1, max1);
			projectPolygonToLine(poly2, normvect, min2, max2);


			//If distance is greater than zero, then there cannot be a collsion.
			float distance = distanceBetweenLines(min1, max1, min2, max2);
			if(distance > 0  || isZero(distance))
				result.overlapped = false;
			else
			{
				distance = -distance;

				if(distance < minOverlap)
				{
					minOverlap = distance;
					result.overlap = normvect;
				}
			}

			/***	Calculate if there WILL be a collision next frame.	***/

			//Make sure there is a velocity defined.
			if(velocity)
			{
				float velocityProj = normvect.dot2D(*velocity);

				//Stick on velocity.
				if (velocityProj < 0)
					min1 += velocityProj;
				else
					max1 += velocityProj;

				//Now do it again.

				distance = distanceBetweenLines(min1, max1, min2, max2);

				if(distance > 0 || isZero(distance))
					result.willCollide = false;

				distance = fabs(distance);

				if(distance < minInterval)
				{
					minInterval = distance;
					responseVect = normvect;

			//		if(approach.dot2D(responseVect) < 0)
			//			responseVect *= -1;
				}
			}
			else
				result.willCollide = false;

			if(!result.willCollide && !result.overlapped)
				//No collision possible, end calculation.
				return false;

			//Collision still possible, continue on.
		}
	}

	if(results)
	{
		if(result.overlapped)
		{
			//if the vector's the wrong way, we'll get some crazy jumping
			if(approach.dot2D(result.overlap) < 0)
				result.overlap *= -1;

			//Apply the overlap distance to the vector
			result.overlap *= minOverlap;
		}

		if(result.willCollide)
		{
			//prevent weird jumps
			if(approach.dot2D(responseVect) < 0)
				responseVect *= -1;

			result.responseVect = responseVect * minInterval;
		}
		*results = result;
	}

	return true;
}
#pragma endregion

#pragma region extra
void calc::projectPolygonToLine(const polygon& poly, const vector3& line,
								float& min, float& max)
{
	if(!poly.vertexCount)
		return;

	float dot = min = max = poly.vertecies[0].coords.dot2D(line);

	for(int c = 0; c < poly.vertexCount; c++)
	{
		dot = poly.vertecies[c].coords.dot2D(line);

		if(dot < min)
			min = dot;
		else if( dot > max)
			max = dot;
	}
}

float calc::distanceBetweenLines(float min1, float max1, float min2, float max2)
{
	//figure out which line is "higher," then do the correct subtraction.
	if(min1 < min2)
		return min2 - max1;
	else
		return min1 - max2;
}
#pragma endregion
#pragma endregion

#pragma region other intersect

bool calc::sphereOverlap(const vector3& pt1, float radius1, const vector3& pt2, float radius2)
{
	if((pt2 - pt1).length() < radius1 + radius2)
		return true;
	else
		return false;
}

bool calc::lineIntersectPoly(const vector3& pt1, const vector3& pt2,
							 const polygon& poly, float* distance)
{
	//Do the same general idea as the poly collision checks
	vector3 lineVect = (pt2 - pt1).normalized();

	float min = 0, max = 0;
	projectPolygonToLine(poly, lineVect, min, max);

	//If the poly min is between both points on the line, we have what we want.
		//Note that this means if the line originates INSIDE the polygon,
		//it will not return true.
	if(min < pt2.dot2D(lineVect) && min > pt1.dot2D(lineVect))
	{
		if(distance)
			*distance = min;
		return true;
	}
	else
		return false;
}

bool calc::polyIntersectRect(const polygon& poly, const rect& box,
					   vector3* overlapVect)
{
	vector3 checkAngle(1,0,0), overlap;

	float min = 0, max = 0;
	projectPolygonToLine(poly, checkAngle, min, max);
	float distance = distanceBetweenLines(min, max, (float)box.left, (float)box.right);
	bool collided = true;

	//Less than zero tolerance
	if(distance < EPSILON)
	{
		if(poly.center.coords.x > box.left)
			overlap.x = -distance;
		else if(poly.center.coords.x < box.right)
			overlap.x = distance;
	}
	else
		collided = false;

	if(collided)
	{
		checkAngle = vector3(0,1,0);
		projectPolygonToLine(poly, checkAngle, min, max);
		distance = distanceBetweenLines(min, max, (float)box.top, (float)box.bottom);

		if(distance < EPSILON)
		{
			if(poly.center.coords.y > box.top)
				overlap.y = -distance;
			else if(poly.center.coords.y < box.bottom)
				overlap.y = distance;
		}
		else
			return false;

		//It'll do some crazy stuff if the overlap vect gives both overlaps, and gets
		//applied straight up.
		if(fabs(overlap.x) > fabs(overlap.y))
			overlap.x = 0;
	}

	if(overlapVect)
	{
		*overlapVect = overlap;
	}

	return collided;
}

void calc::rectToPoly(const rect &box, polygon* poly)
{
	vector3 bc = vector3(
		(float)box.bottom, (float)box.right, 0)
		- vector3((float)box.top, (float)box.left, 0);

	polygon boxy;
	boxy.maxRadius = (bc - vector3((float)box.top, (float)box.left, 0)).length();

	boxy.vertexCount = 4;
	boxy.vertecies = new objectPoint[4];

	boxy.vertecies[0] = objectPoint((float)box.top, (float)box.left);
	boxy.vertecies[1] = objectPoint((float)box.top, (float)box.right);
	boxy.vertecies[2] = objectPoint((float)box.bottom, (float)box.right);
	boxy.vertecies[3] = objectPoint((float)box.bottom, (float)box.left);

	*poly = boxy;

}

void calc::projectRectToLine(const rect& box, const vector3& line, float& min, float& max)
{
	if(isZero(line.x))
	{
		min = box.top * line.y;
		max = box.bottom * line.y;

		return;
	}
	else if(isZero(line.y))
	{
		min = box.left * line.x;
		max = box.right * line.x;

		return;
	}

	min = (float)_HUGE;
	max = -(float)_HUGE;

	float dots[4] = 
	{
		vector3((float)box.left, (float)box.top).dot2D(line),
		vector3((float)box.right, (float)box.top).dot2D(line),
		vector3((float)box.right, (float)box.bottom).dot2D(line),
		vector3((float)box.left, (float)box.bottom).dot2D(line)
	};

	for(int c = 0; c < 4; c++)
	{
		if(dots[c] < min)
			min = dots[c];
		if(dots[c] > max)
			max = dots[c];
	}
}

#pragma endregion