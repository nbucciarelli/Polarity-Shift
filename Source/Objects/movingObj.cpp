#include "movingObj.h"
#include "..\Helpers\physics.h"
#include "../engines/ctileengine.h"
#include <cmath>
#include <windows.h>

movingObj::movingObj(uint otype) : baseObj(otype, true), onGround(false)
{
}

movingObj::~movingObj(void)
{
}

void movingObj::update(float dt)
{
	onGround = mapCollisionCheck();

	//Apply gravity.
	if(!onGround)
		velocity.y += GRAVITY * dt;
	velocity += acceleration * dt;
	position += velocity * dt;

	angVel += angAcc * dt;
	angPos += angVel * dt;

	//zeroDrift checks for zero tolerance on all the floats, and sets to zero.
	velocity.zeroDrift();
	acceleration.zeroDrift();
	angVel.zeroDrift();
	angAcc.zeroDrift();


	//Hax for ground.
	if(1 > 500)
	{
		position.y = 500;
		velocity.y = 0;
		acceleration.y = 0;
	}

	for(int c = 0; c < 3; c++)
		if(angPos.e[c] > _2PI) angPos.e[c] -= _2PI;

	baseObj::update(dt);
}

bool movingObj::checkCollision(baseObj* obj, polyCollision* result)
{
	polyCollision holder;

	//Make sure both objects have their bounding sphere specified
	if(collisionPoly->maxRadius > 0 && obj->getMaxRadius() > 0)
	{
		//Check to see if the objects are close enough for a potential collision.
		if(!calc::sphereOverlap(this->position, collisionPoly->maxRadius,
				obj->getPosition(), obj->getMaxRadius()))
			return false;
	}

	collisionHandling(*obj->getCollisionPoly(), holder, obj);

	if(result)
		*result = holder;

	return true;	
}

bool movingObj::checkTerrainCollision(rect& colBounds)
{

	return false;
}

bool movingObj::mapCollisionCheck()
{
	const vector<RECT>& collisionBox = CTileEngine::GetInstance()->GetCollisions();

	const polygon& poly = *getCollisionPoly();
//	polygon box;
//	vector3 overlap;

	//polyCollision result;

	bool yHit = false;
	const rect* box = NULL;
	const vector3* point = NULL;

	float distancex = 0, distancey = 0, minDistance = (float)_HUGE;
	bool Y = false;

	for(unsigned c = 0; c < collisionBox.size(); c++)
	{
		box = (rect*)&collisionBox[c];

		RECT intersect;
		if(!IntersectRect(&intersect,
			(RECT*)(&getCollisionRect()),
			(RECT*)(box))
			)
			continue;

		if(intersect.bottom - intersect.top > intersect.right - intersect.left)
		{	//Side hit
			if(intersect.left < position.x) //Left side
				position.x += intersect.right - intersect.left + 1;
			else //right side
				position.x -= intersect.right - intersect.left + 1;
		}
		else //Vertical hit
		{
			if(intersect.top < position.y/* && !onGround*/) //top hit, don't react
				position.y += intersect.bottom - intersect.top + 1; //if standing
			else if(intersect.bottom > position.y)
			{ //bottom hit.  You're standing.
				position.y -= intersect.bottom - intersect.top + 1;
				yHit = true;
				velocity.y = 0;
				acceleration.y = 0;
			}
		}

	}

	return yHit;
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
	//if(result.overlapped)
			//position += result.overlap;
	//Affect velocity based on collision.
	vector3 mod = result.responseVect.normalized() * result.responseVect.dot2D(velocity) * frameTime;
	velocity += mod;
	if(obj && obj->IsMovable())
		((movingObj*)obj)->modVel(mod);

	return true;
}