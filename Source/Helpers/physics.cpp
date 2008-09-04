#include "datatypes.h"
#include "physics.h"
#include <cmath>

#include <d3dx9.h>

#pragma comment(lib, "d3dx9.lib")


#pragma region matrix rotation
//TODO:  RECALCULATE MATRIX EQUATIONS, get rid of d3dx references

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

bool calc::isZero(float val, float epsilon)
{
	if(fabs(val) < epsilon)
		return true;
	else
		return false;
}

#pragma region polygon collision

bool calc::polygonCollision(const polygon& poly1, const polygon& poly2, const vector3* velocity, polyCollision* results)
{
	polyCollision result;
	result.overlapped = result.willCollide = true;

	const polygon* poly[2] = { &poly1, &poly2 };

	vector3 approach = (poly1.center.coords - poly2.center.coords);
	vector3 responseVect;
	float min1, max1, min2, max2;
	min1 = max1 = min2 = max2 = 0;

	float minInterval = (float)_HUGE;
	float maxOverlap = 0;

	//code-efficient (read: less typing) way of checking through both polys.
	for(int p = 0; p < 2; p++)
	{
		for(int c = 0; c < poly1.vertexCount; c++)
		{
			//get the current edge
			vector3 edgevector;
			if(c)
				edgevector = poly[p]->vertecies[c].coords - poly[p]->vertecies[c-1].coords;
			else
				edgevector = poly[p]->vertecies[c].coords - poly[p]->vertecies[poly1.vertexCount - 1].coords;

			//and the normal of the edge
			vector3 normvect = vector3(edgevector.y, -edgevector.x, 0);
			normvect.normalize();

			//Squish polygons to current edge vector
			projectPolygonToLine(poly1, normvect, min1, max1);
			projectPolygonToLine(poly2, normvect, min2, max2);


			//If distance is greater than zero, then there cannot be a collsion.
			float distance = distanceBetweenLines(min1, max1, min2, max2);
			if(distance > 0)
				result.overlapped = false;

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

				if(distance > 0)
					result.willCollide = false;

				if(!result.willCollide && !result.overlapped)
					//No collision possible, end calculation.
					return false;

				//Collision still possible, continue on.

				distance = fabs(distance);

				if(distance < minInterval)
				{
					minInterval = distance;
					responseVect = normvect;

			//		if(approach.dot2D(responseVect) < 0)
			//			responseVect *= -1;
				}
			}
			else if(!result.willCollide)
				return false;
		}
	}

	if(results)
	{
		if(result.overlapped)
		{
			projectPolygonToLine(poly1, *velocity, min1, max1);
			projectPolygonToLine(poly2, *velocity, min2, max2);

			float distance = distanceBetweenLines(min1, max1, min2, max2);

			result.overlap = (*velocity).normalized() * distance;
		}

		result.responseVect += responseVect * minInterval;
		*results = result;
	}

	return true;
}

void calc::projectPolygonToLine(const polygon& poly, const vector3& line,
								float& min, float& max)
{
	if(!poly.vertecies)
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

#pragma region other intersect

bool calc::sphereOverlap(const vector3& pt1, float radius1, const vector3& pt2, float radius2)
{
	if((pt2 - pt1).length() < radius1 + radius2)
		return true;
	else
		return false;
}

bool lineIntersectPoly(vector3& rayStart, vector3& rayVect, polygon& poly)
{
	return false;
}

#pragma endregion