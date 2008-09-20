#include "magnetGun.h"
#include "../helpers/physics.h"
#include "playerObj.h"
#include "objManager.h"
#include "../wrappers/mouse.h"
#include "../engines/CTileEngine.h"
#include <vector>
#include <cmath>

#define baseRadius 10
#define HOLDRANGE 300

magnetGun::magnetGun() : beamWidthFactor(1), theMouse(mouse::getInstance()),
mode(0), target(0)
{
}

magnetGun::~magnetGun()
{
}

void magnetGun::openFire(const vector3 *trajectory, int fireMode)
{
	//If already firing...do some checks.
	if(isActive)
	{
		if(!target || mode == MAG_HOLD || fireMode == mode)
			return;
		else
			fireMode = MAG_HOLD;
	}

	//Start by putting the range on the trajectory
	//Also, the perpendicular vector
	vector3 fireLine;
	
	if(trajectory)
		fireLine = trajectory->normalized();
	//vector3 radiusLine(fireLine.y, -fireLine.x, 0);
	else
	{
		fireLine = (theMouse->getPos() - pos).normalized();
	}

	fireLine *= (float)range;
	//radiusLine *= baseRadius * beamWidthFactor;

	isActive = getTarget(fireLine + pos);

	if(isActive)
		mode = fireMode;
	else
	{
		isActive = false;
		mode = MAG_OFF;
		return;
	}

	switch(mode)
	{
	case MAG_PUSH:
	case MAG_PULL:
	case MAG_HOLD:
		//target->setAcc(vector3());
		//target->setVel(vector3());
		break;
	default:
		break;
	}
}

void magnetGun::ceaseFire(int dat)
{
	if(mode == MAG_HOLD)
	{
		if(dat == MAG_PUSH)
			mode = MAG_PULL;
		else if(dat == MAG_PULL)
			mode = MAG_PUSH;

		if(mode != MAG_HOLD)
			return;
	}
	else if(!isActive)
		return;

	if(target)
	target->setAcc(vector3());
	target = NULL;
	mode = MAG_OFF;
	isActive = false;
}

bool magnetGun::getTarget(const vector3& farPoint)
{
	int selection = -1;
	float minDist = (float)_HUGE, dist = 0;

	vector3 perp = (farPoint - pos).normalized();
	perp = vector3(perp.y, -perp.x);
	float centerLineProj = pos * perp;
	float radius = baseRadius * beamWidthFactor;

	//Just to sneak in this variable.  Makes it look less scary in class declaration.
	//And magical, too.
	static const std::vector<baseObj*>& objList = objManager::getInstance()->getList();

//	float limit = levelLimiter((farPoint - pos).normalized());

//	if(range < limit)
float		limit = (float)range;

	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(owner == objList[c])
			continue;

		if(objList[c]->IsMovable()
			&&  calc::lineIntersectPoly(pos, farPoint,
									   *objList[c]->getCollisionPoly(), &dist)
			&& fabs(dist) < minDist)//  && dist > 0)
		{
			//So, we have a potential.  Let's make sure it's within radius.
			//If not, this isn't it.  Keep going.
			float miny, maxy;
			calc::projectPolygonToLine(*objList[c]->getCollisionPoly(), perp, miny, maxy);

			if(centerLineProj + radius < miny || centerLineProj - radius > maxy)
				continue;

			minDist = dist;
			selection = c;
		}
	}

	if(selection != -1 && minDist < limit)
	{
		target = (movingObj*)(objList[selection]);
		return true;
	}
	else
	{
		static std::vector<RECT>& magRect = CTileEngine::GetInstance()->GetMagnets();

		minDist = (float)_HUGE;
		polygon poly;
		for(unsigned c = 0; c < magRect.size(); c++)
		{
			calc::rectToPoly(*(rect*)&magRect[c], &poly);

			if(calc::lineIntersectPoly(pos, farPoint,
				poly, &dist)
				&& fabs(dist) < minDist)//  && dist > 0)
			{
				//So, we have a potential.  Let's make sure it's within radius.
				//If not, this isn't it.  Keep going.
				float miny, maxy;
				calc::projectPolygonToLine(poly, perp, miny, maxy);

				delete[] poly.vertecies;
				poly.vertecies = 0;

				if(centerLineProj + radius < miny || centerLineProj - radius > maxy)
					continue;

				minDist = dist;
				selection = c;
			}
		}
		if(selection != -1 && minDist < limit)
		{
			target = 0;
			return true;
		}
	}

	return false;
}

//float magnetGun::levelLimiter(const vector3& traj)
//{
//	static std::vector<RECT>& colRect = CTileEngine::GetInstance()->GetCollisions();
//
//	vector3 perp = vector3(traj.y, -traj.x);
//
//	float dist, minDist = (float)_HUGE;
//	float posDotP = pos.dot2D(perp);
//	float posDot = pos.dot2D(traj);
//	polygon poly;
//
///*	for(unsigned c = 0; c < colRect.size(); c++)
//	{
//		calc::rectToPoly(*(rect*)&colRect[c], &poly);
//
//		if(calc::lineIntersectPoly(pos, farPoint,
//									   poly, &dist)
//			&& fabs(dist) < minDist)//  && dist > 0)
//		{
//			float miny, maxy;
//			calc::projectPolygonToLine(*objList[c]->getCollisionPoly(), perp, miny, maxy);
//
//			if(pos + radius < miny || centerLineProj - radius > maxy)
//				continue;
//
//			minDist = dist;
//			selection = c;
//		}
//	}*/
//
//	return minDist - posDot;
//}

void magnetGun::update(float dt)
{
	pos = owner->getPosition();
	pos.y += 15;

	if(mode != MAG_OFF)
	{
		if(target)
		{
		target->setVel(vector3());
		target->setAcc(vector3(0,-GRAVITY));
		}
		else
		{
			((movingObj*)owner)->setVel(vector3());
			((movingObj*)owner)->setAcc(vector3(0,-GRAVITY));
		}

		float len = 0;
		vector3 traj = (theMouse->getPos() - pos).normalized();

		switch(mode)
		{
		case MAG_PUSH:
			if(target)
				target->modPos(traj * (power * dt));
			else
				owner->modPos(traj * -(power * dt));
			break;
		case MAG_PULL:
			if(target)
				target->modPos(traj * -(power * dt));
			else
				owner->modPos(traj * (power * dt));
			break;
		case MAG_HOLD:
			traj = (theMouse->getPos() - target->getPosition());
			 len = traj.length();
			if(len > HOLDRANGE)
				ceaseFire();
			else if(len > 10)
				target->modPos(traj.normalized() * (power * 0.75f * dt));
			break;
		default:
			break;
		}
	}
}

void magnetGun::render() const
{
}