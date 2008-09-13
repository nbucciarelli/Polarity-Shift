#include "movingObj.h"
#include "..\Helpers\physics.h"
#include "../engines/ctileengine.h"
#include <cmath>
#include <windows.h>

movingObj::movingObj(uint otype) : baseObj(otype, true), onSurface(false),
							leftWall(false),rightWall(false),topWall(false)
{
}

movingObj::~movingObj(void)
{
}

void movingObj::update(float dt)
{
	velocity += acceleration * dt;
	position += velocity * dt;

	angVel += angAcc * dt;
	angPos += angVel * dt;
	
	mapCollisionCheck();

	//Apply gravity.
	if(!onSurface)
		velocity.y += GRAVITY * dt;
	else
	{
		if( velocity.y > 0 )
			velocity.y = 0;
		if(acceleration.y > 0)
			acceleration.y = 0;

	}

	if(leftWall)
	{
		if( velocity.x < 0 )
			velocity.x = 0;
		if( acceleration.x < 0 )
			acceleration.x = 0;
	}
	if(rightWall)
	{
		if( velocity.x > 0 )
			velocity.x = 0;
		if( acceleration.x > 0 )
			acceleration.x = 0;
	}
	if(topWall)
	{
		if(velocity.y < 0 )
			velocity.y = 0;
		if(acceleration.y < 0 )
			acceleration.y = 0;
	}

	//zeroDrift checks for zero tolerance on all the floats, and sets to zero.
	velocity.zeroDrift();
	acceleration.zeroDrift();
	angVel.zeroDrift();
	angAcc.zeroDrift();

	for(int c = 0; c < 3; c++)
		if(angPos.e[c] > _2PI) angPos.e[c] -= _2PI;

	baseObj::update(dt);
}

bool movingObj::checkCollision(baseObj* obj, polyCollision* result)
{
	polyCollision holder;

	const polygon* thisPoly = getCollisionPoly();

	//Make sure both objects have their bounding sphere specified
	if(thisPoly->maxRadius > 0 && obj->getMaxRadius() > 0)
	{
		//Check to see if the objects are close enough for a potential collision.
		if(!calc::sphereOverlap(this->position, thisPoly->maxRadius,
				obj->getPosition(), obj->getMaxRadius()))
			return false;
	}

	collisionHandling(*obj->getCollisionPoly(), holder, obj);

	if(result)
		*result = holder;

	return true;	
}

bool movingObj::mapCollisionCheck()
{
	const vector<RECT>& collisionBox = CTileEngine::GetInstance()->GetCollisions();

	const polygon& poly = *getCollisionPoly();
//	polygon box;
//	vector3 overlap;

	//polyCollision result;

#define EXCESS 2

	const rect* box = NULL;
	rect& thisBox = getCollisionRect();

	rect overBox = thisBox;
	overBox.top -= EXCESS;
	overBox.left -= EXCESS;
	overBox.right += EXCESS;
	overBox.bottom += EXCESS;

	onSurface = topWall = rightWall = leftWall = false;

	for(unsigned c = 0; c < collisionBox.size(); c++)
	{
		box = (rect*)&collisionBox[c];

		RECT intersect;
		if(!IntersectRect(&intersect,
			(RECT*)&overBox,
			(RECT*)(box))
			)
		{
			continue;
		}

		//First, check if vertical hit
		if(intersect.bottom - intersect.top < intersect.right - intersect.left)
		{
			if(intersect.bottom == overBox.bottom) //bottom hit
			//if(intersect.bottom == overBox.bottom && intersect.top < thisBox.bottom)
			{
				if(!onSurface)
				{
				position.y += intersect.top - intersect.bottom + EXCESS;
				onSurface = true;
				}
			}
			else //if(intersect.top + EXCESS > thisBox.bottom)
				onSurface = false;

			if(intersect.top == overBox.top) //Top hit
			//if(intersect.top == overBox.top && intersect.bottom > thisBox.top)
			{
				if(!topWall)
				{
				position.y += intersect.bottom - intersect.top - EXCESS;
				topWall = true;
				}
				
			}
			else //if(intersect.bottom - EXCESS < thisBox.top)
				topWall = false;
		}
		else
			onSurface = topWall = false;
		
		if(intersect.bottom - intersect.top > intersect.right - intersect.left)
			//horizontal hit
		{
			if(intersect.left == overBox.left) //left hit
			{
				if(!leftWall)
				{
				position.x += intersect.right - intersect.left + EXCESS;
				leftWall = true;
				}
			}
			else //if(intersect.right - EXCESS > thisBox.left)
				leftWall = false;

			if(intersect.right == overBox.right)
			{
				if(!rightWall)
				{
				position.x += intersect.left - intersect.right - EXCESS;
				rightWall = true;
				}
			}
			else //if(intersect.right - EXCESS < thisBox.right)
				rightWall = false;
		}
		else
			leftWall = rightWall = false;
	}

	return true;
}

bool movingObj::collisionHandling(const polygon& poly, polyCollision& result, baseObj* obj)
{
	//if the collision check returns false, there's nothing else to do.
	vector3 vel;
	if(obj)
		vel = (velocity - obj->getVelocity())*frameTime;
	else
		vel = velocity;

	if(!calc::polygonCollision(poly, *getCollisionPoly(),
		&vel, &result))
			return false;

	//move out of collision.
	if(obj && obj->IsMovable())
	{
		if(result.willCollide)
		{
			position += result.responseVect * -0.5f;
			obj->modPos(result.responseVect * 0.5f);
		}
		if(result.overlapped)
		{
			position += result.overlap * -0.5f;
			obj->modPos(result.overlap * 0.5f);

		//	obj->getAngPos();
		}
	}
	else
	{
		if(result.willCollide)
			position += result.responseVect;

		if(result.overlapped)
			position += result.overlap;
		
	}

	//Affect velocity based on collision.
	vector3 mod = result.responseVect.normalized() * result.responseVect.dot2D(velocity) * frameTime;
	velocity += mod;
	if(obj && obj->IsMovable())
		((movingObj*)obj)->modVel(mod);

	return true;
}