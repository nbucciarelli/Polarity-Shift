#include "movingObj.h"
#include "..\Helpers\physics.h"
#include "../engines/ctileengine.h"
#include <cmath>
#include <windows.h>

#define friction 0.01f

movingObj::movingObj(uint otype) : baseObj(otype, true), onSurface(false),
leftWall(false),rightWall(false),topWall(false),stoodOn(0)
{
}

movingObj::~movingObj(void)
{
}

void movingObj::update(float dt)
{
	//mapCollisionCheck();

	//Apply gravity.
	if(!isOnSurface())
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

	if(!calc::isZero(velocity))
	{
		for(int c = 0; c < 3; c++)
		{
			if(velocity.e[c] > 0)
				velocity.e[c] -= 0.1f;
			else if(velocity.e[c] < 0)
				velocity.e[c] += 0.1f;
		}
	}
	if(!calc::isZero(acceleration))
	{
		for(int c = 0; c < 3; c++)
		{
			if(acceleration.e[c] > 0)
				acceleration.e[c] -= 0.1f;
			else if(acceleration.e[c] < 0)
				acceleration.e[c] += 0.1f;
		}
	}

	velocity += acceleration * dt;
	position += velocity * dt;

	angVel += angAcc * dt;
	angPos += angVel * dt;

	for(int c = 0; c < 3; c++)
		if(angPos.e[c] > _2PI) angPos.e[c] -= _2PI;

	if(!calc::isZero(velocity.x))
	{
		if(velocity.x > 0)
			acceleration.x -= friction;
		else
			acceleration.x += friction;
	}

	if(stoodOn && (onSurface ||
		(
		!calc::sphereOverlap(position, getMaxRadius(),
							stoodOn->position,
							stoodOn->getMaxRadius())
		&&
		!calc::polygonCollision(*stoodOn->getCollisionPoly(), *getCollisionPoly(),
								&vector3())
		)))
	{
		stoodOn = 0;
	}

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

	//collisionHandling(*obj->getCollisionPoly(), holder, obj);
	vector3 vel;
	if(obj)
		vel = (velocity - obj->getVelocity())*frameTime;
	else
		vel = velocity;

	if(!calc::polygonCollision(*obj->getCollisionPoly(), *thisPoly,
		&vel, &holder))
		return false;

	movingObj* other = (movingObj*)obj;
	vector3 y = vector3(0,1);
	vector3 x = vector3(1);

	if (other->isOnSurface() && holder.responseVect * y > 0 && !other->isInStack(this))
	{
		stoodOn = other;
	}
	else if(isOnSurface() && holder.responseVect * y < 0 && !isInStack(other))
	{
		other->stoodOn = this;
	}
	else
	if (other->isOnSurface() && holder.overlap * y > 0 && !other->isInStack(this))
	{
		stoodOn = other;
	}
	else if(isOnSurface() && holder.overlap * y < 0 && !isInStack(other))
	{
		other->stoodOn = this;
	}

	/*if((holder.overlapped && holder.overlap * y > 0)
		|| (holder.willCollide && holder.responseVect * y > 0))
			stoodOn = other;
	else if((holder.overlapped && holder.overlap * y < 0)
		|| (holder.willCollide && holder.responseVect * y < 0))
		other->stoodOn = this;*/

	holder.overlap *= 0.5f;
	holder.responseVect *= 0.5f;
		
	other->collisionQueue.push(new colSet(holder, this));

	holder.overlap *= -1;
	holder.responseVect *= -1;

	collisionQueue.push(new colSet(holder, obj));

	if(result)
		*result = holder;

	return true;	
}

bool movingObj::mapCollisionCheck()
{
	const vector<RECT>& collisionBox = CTileEngine::GetInstance()->GetCollisions();

	const polygon& poly = *getCollisionPoly();

#define EXCESS 2

	const rect* box = NULL;
	rect thisBox;
	float min, max;

	calc::projectPolygonToLine(poly, vector3(1), min, max);
	thisBox.left = (int)min;
	thisBox.right = (int)max;
	calc::projectPolygonToLine(poly, vector3(0,1), min, max);
	thisBox.top = (int)min;
	thisBox.bottom = (int)max;

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
			{
				if(!isOnSurface())
				{
					position.y += intersect.top - intersect.bottom + EXCESS;
					onSurface = true;
				}
			}
			else
				onSurface = false;

			if(intersect.top == overBox.top) //Top hit
			{
				if(!topWall)
				{
					position.y += intersect.bottom - intersect.top - EXCESS;
					topWall = true;
				}

			}
			else
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
			else
				leftWall = false;

			if(intersect.right == overBox.right)
			{
				if(!rightWall)
				{
					position.x += intersect.left - intersect.right - EXCESS;
					rightWall = true;
				}
			}
			else
				rightWall = false;
		}
		else
			leftWall = rightWall = false;
	}
	return true;
}

bool movingObj::collisionHandling(const polygon& poly, polyCollision& result, baseObj* obj)
{
	vector3 vel;
	if(obj)
		vel = (velocity - obj->getVelocity())*frameTime;
	else
		vel = velocity;

	if(!calc::polygonCollision(poly, *getCollisionPoly(),
		&vel, &result))
		return false;

	//move out of collision.
#pragma region old collision response
/*	if(obj && obj->IsMovable())
	{
		movingObj* movey = (movingObj*)obj;

		if(result.willCollide)
		{
			if(movey->onSurface && result.responseVect * vector3(0,1) >= 0)
			{
				position += vector3(result.responseVect.x*-0.5f,-result.responseVect.y);
				movey->position += vector3(result.responseVect.x*0.5f);
				onSurface = true;
			}
			else if(onSurface && result.responseVect * vector3(0,-1) >= 0)
			{
				position += vector3(result.responseVect.x * -0.5f);
				movey->position += vector3(result.responseVect.x *0.5f, result.responseVect.y);
				movey->onSurface = true;
			}
			else
			{
				position += result.responseVect * -0.5f;
				obj->modPos(result.responseVect * 0.5f);
			}
		}
		if(result.overlapped)
		{
			vector3 overlap;
			//if(movey->topWall && result.responseVect * vector3(0,-1) <= 0)
			if(movey->onSurface && result.overlap * vector3(0,1) > 0)
				onSurface = true;
			else if(onSurface && result.overlap * vector3(0,-1) > 0)
				movey->onSurface = true;
			else if(movey->topWall && result.overlap * vector3(0,-1) > 0)
				topWall = true;
			else if(topWall && result.overlap * vector3(0,1) > 0)
				movey->topWall = false;
			else
				overlap.y = result.overlap.y;

			if(movey->leftWall && result.overlap * vector3(1) > 0)
				leftWall = true;
			else if(leftWall && result.overlap * vector3(-1) > 0)
				movey->leftWall = true;
			else if(movey->rightWall && result.overlap * vector3(-1) > 0)
				rightWall = true;
			else if(movey->rightWall && result.overlap * vector3(1) > 0)
				movey->rightWall = true;
			else
				overlap.x = result.overlap.x;

			position += overlap * -0.5f;
			obj->modPos(overlap * 0.5f);
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
	velocity -= mod * 0.4f;
	if(obj && obj->IsMovable())
		((movingObj*)obj)->modVel(mod * 0.4f);
*/
#pragma endregion

	if(!obj || !obj->IsMovable())
	{
		if(result.willCollide)
			position += result.responseVect;

		if(result.overlapped)
			position += result.overlap;

		vector3 mod = result.responseVect.normalized()
			* result.responseVect.dot2D(velocity) * frameTime;
		velocity -= mod * 0.4f;
		((movingObj*)obj)->modVel(mod * 0.4f);
	}
	else //Moving Object collision response
	{
		movingObj* other = (movingObj*)obj;
		vector3 y = vector3(0,1);
		vector3 x = vector3(1);

		if (other->onSurface && result.responseVect * y > 0)
		{
			//obj is on ground, and response tells it to push down.
			position.x += result.responseVect.x * -0.5f;
			position.y += -result.responseVect.y;
			
			if(velocity.y > 0)
				velocity.y = 0;

			other->position.x -= result.responseVect.x * 0.5f;
		}
		else if(onSurface && result.responseVect * y < 0)
		{
			//this is on ground, and response says push up.
			position.x -= result.overlap.x;

			other->position.y += result.overlap.y;
			other->position.x += result.overlap.x * 0.5f;

			if(other->velocity.y > 0)
				other->velocity.y = 0;
		}
		else
		{
			position -= result.responseVect * 0.5f;
			other->position += result.responseVect * 0.5f;
		}


		if (other->onSurface && result.overlap * y > 0)
		{
			//obj is on ground, and response tells it to push down.
			position.x += result.overlap.x * -0.5f;
			position.y += -result.overlap.y;
			
			if(velocity.y > 0)
				velocity.y = 0;

			other->position.x -= result.overlap.x * 0.5f;
		}
		else if(onSurface && result.overlap * y < 0)
		{
			//this is on ground, and response says push up.
			position.x -= result.overlap.x;

			other->position.y += result.overlap.y;
			other->position.x += result.overlap.x * 0.5f;

			if(other->velocity.y > 0)
				other->velocity.y = 0;
		}
		else
		{
			position -= result.overlap * 0.5f;
			other->position += result.overlap * 0.5f;
		}

		//velocity now.
		vector3 vel = result.responseVect.normalized()
			* result.responseVect.dot2D(velocity + other->velocity) * frameTime * 0.4f;

		if(onSurface && other->onSurface && vel * y > 0)
		{
			velocity.x -= vel.x;
			other->velocity.x += vel.x;
		}
		else if(onSurface && vel * y > 0)
		{
			velocity.x -= vel.x;
			other->velocity += vel;
		}
		else if(other->onSurface && vel * y < 0)
		{
			velocity -= vel;
			other->velocity.x += vel.x;
		}
		else
		{
			velocity -= vel;
			other->velocity += vel;
		}
	}

	return true;
}

void movingObj::collisionReact()
{
#define current collisionQueue.front()->result
#define currentObj collisionQueue.front()->obj
#define moving ((movingObj*)collisionQueue.front()->obj)
	
	vector3 y = vector3(0,1);
	vector3 x = vector3(1);

	while(!collisionQueue.empty())
	{
		if(current.overlapped)
		{
			if(isOnSurface() && current.overlap * y > 0)
				position.x += current.overlap.x;
			else
				position += current.overlap;
				
		}

		if(current.willCollide)
		{
			if(isOnSurface() && current.responseVect * y > 0)
				position.x += current.responseVect.x;
			else
				position += current.responseVect;
		}

		delete collisionQueue.front();
		collisionQueue.pop();
	}
}

bool movingObj::isOnSurface() const
{
	if(stoodOn)
		return stoodOn->isOnSurface();
	else
		return onSurface;
}

bool movingObj::isInStack(const baseObj* obj)
{
	if(!stoodOn)
		return false;

	if(obj == stoodOn)
		return true;
	else
		return stoodOn->isInStack(obj);
}