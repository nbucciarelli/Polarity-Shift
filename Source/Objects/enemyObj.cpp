#include "enemyObj.h"
#include <cmath>

enemyObj::enemyObj() : maxVel(500), maxAcc(50), movingObj(OBJ_ENEMY)
{}

enemyObj::~enemyObj()
{}

void enemyObj::update(float dt)
{
	movingObj::update(dt);
	// IF ANIMATION IS RENDERING, NEEDS TO UPDATE THE ANIMATION MANAGER FOR ACTUAL ANIMATION

	if(maxAcc && fabs(acceleration.x) > maxAcc)
	{
		if(acceleration.x < 0)
			acceleration.x = -maxAcc;
		else
			acceleration.x = maxAcc;
	}

	if(maxVel && fabs(velocity.x) > maxVel)
	{
		if(velocity.x < 0)
			velocity.x = -maxVel;
		else
			velocity.x = maxVel;

		acceleration.x = 0;
	}
}