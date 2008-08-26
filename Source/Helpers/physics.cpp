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

#pragma region polygon collision

bool calc::polygonCollision(const polygon& poly1, const polygon& poly2, vector3* impactVect)
{
	float min1, max1, min2, max2;
	min1 = max1 = min2 = max2 = 0;
	for(int c = 1; c < poly1.vertexCount; c++)
	{
		//get the current edge
		vector3 edgevector = poly1.vertecies[c].coords - poly1.vertecies[c].coords;

		//and the normal of the edge
		vector3 normvect = vector3(edgevector.y, -edgevector.x, 0);

		//Squish poly 1 onto the normal vector.
		projectPolygonToLine(poly1, normvect, min1, max1);
		//Squish poly 2 onto the normal vector.
		projectPolygonToLine(poly2, normvect, min2, max2);


		//If distance is less than zero, then we have a collision on this edge.
		float distance = distanceBetweenLines(min1, max1, min2, max2);
		if( distance <= 0)
		{
			if(impactVect) //If the calling function wants the vect
			{
				//Since Z is unused in this calculation, it is used for overlap magnitude.
				*impactVect = normvect;
				impactVect->z = distance;
			}

			return true;
		}
		//else continue the loop
	}

	//Made it through the loop?  No collision.
	return false;
}

void calc::projectPolygonToLine(const polygon& poly, const vector3& line,
								float& min, float& max)
{
	if(!poly.vertecies)
		return;

	float dot = min = max = poly.vertecies[0].coords.dot2D(line);

	for(int c = 1; c < poly.vertexCount; c++)
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