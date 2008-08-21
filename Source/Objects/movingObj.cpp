#include "movingObj.h"
#include "..\Helpers\physics.h"

movingObj::movingObj(uint otype) : baseObj(otype), boundingSpace(NULL)
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

	for(int c = 0; c < 3; c++)
		if(angPos.e[c] > _2PI) angPos.e[c] -= _2PI;

	baseObj::update(dt);
}

bool movingObj::checkCollision(baseObj* obj, rect* pass)
{
	bool hit = baseObj::checkCollision(obj, pass);

	boundsCheck();

	return hit;
}

void movingObj::boundsCheck()
{
	//if(boundingSpace)
	//{
	//	rect col = getCollisionRect();

	//	if(col.left < boundingSpace->left)
	//	{
	//		position.x += 1;
	//	}
	//	else if(col.right > boundingSpace->right)
	//	{
	//		position.x -= 1;
	//	}

	//	if(col.top < boundingSpace->top)
	//	{
	//		position.y += 1;
	//	}
	//	else if(col.bottom > boundingSpace->bottom)
	//	{
	//		position.y -= 1;
	//	}
	//}
}