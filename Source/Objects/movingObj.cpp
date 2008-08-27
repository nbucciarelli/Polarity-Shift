#include "movingObj.h"
#include "..\Helpers\physics.h"

movingObj::movingObj(uint otype) : baseObj(otype)
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

	//Make sure both objects have their bounding sphere specified.
	if(collisionPoly->maxRadius > 0 && obj->getCollisionPoly())
	{
		//Check to see if the objects are close enough for a potential collision.
		if(!calc::sphereOverlap(this->position, collisionPoly->maxRadius,
				obj->getPosition(), obj->getCollisionPoly()->maxRadius))
			return false;
	}

	//if the collision check returns false, there's nothing else to do.
	if(!calc::polygonCollision(*obj->getCollisionPoly(), *getCollisionPoly(),
		&(velocity/*frameTime*/), &holder))
			return false;

	position += holder.responseVect;

	if(result)
		*result = holder;

	return true;	
}