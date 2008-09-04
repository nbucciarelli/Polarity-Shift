#include "movingObj.h"
#include "..\Helpers\physics.h"

movingObj::movingObj(uint otype) : baseObj(otype, true)
{
}

movingObj::~movingObj(void)
{
}

void movingObj::update(float dt)
{
	//Apply gravity.
	velocity.y += GRAVITY * dt;
	velocity += acceleration * dt;
	position += velocity * dt;

	angVel += angAcc * dt;
	angPos += angVel * dt;

	//Hax for ground.

	if(position.y > 500)
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

	//if the collision check returns false, there's nothing else to do.
	if(!calc::polygonCollision(*obj->getCollisionPoly(), *getCollisionPoly(),
		&((velocity - obj->getVelocity())/*-frameTime*/), &holder))
			return false;

	//move out of collision.
	if(obj->IsMovable())
	{
		if(holder.willCollide)
		{
			position += holder.responseVect * -0.5f;
			obj->modPos(holder.responseVect * 0.5f);
		}
		if(holder.overlapped)
		{
			position += holder.overlap * -0.5f;
			obj->modPos(holder.overlap * 0.5f);
		}
	}
	else
	{
		if(holder.willCollide)
			position += holder.responseVect;

		if(holder.overlapped)
			position += holder.overlap;
		
	}
	//if(holder.overlapped)
			//position += holder.overlap;
	//Affect velocity based on collision.
	vector3 mod = holder.responseVect.normalized() * holder.responseVect.dot2D(velocity) * frameTime;
	velocity -= mod;
	if(obj->IsMovable())
		((movingObj*)obj)->modVel(mod);

	if(result)
		*result = holder;

	return true;	
}

bool movingObj::checkTerrainCollision(rect& colBounds)
{

	return false;
}