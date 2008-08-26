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

	for(int c = 0; c < 3; c++)
		if(angPos.e[c] > _2PI) angPos.e[c] -= _2PI;

	baseObj::update(dt);
}

bool movingObj::checkCollision(baseObj* obj, vector3* impactVect)
{
	vector3 holder;

	bool hit = baseObj::checkCollision(obj, &holder);

	//If the collision check says false, we're done.
	if(!hit)
		return false;

	//Otherwise, it's time to react.  impactVect is the normal of the other obj. impactvect.z
	//is the overlap amount.

	//Jerk position out of the way.
	position += holder * holder.z;

	//Apply the normal force of the surface.  At present, all surfaces are solids.
	//This is calculated by projecting the acceleration onto -impactVector, and negating.
//	acceleration += -(*(-holder.normalize());

	if(impactVect)
	{
		*impactVect = holder;
		return true;
	}

	return true;
}