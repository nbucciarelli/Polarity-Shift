#include "magnetGun.h"
#include "../helpers/physics.h"
#include "playerObj.h"
#include "objManager.h"
#include "../wrappers/mouse.h"
#include "../engines/CTileEngine.h"
#include "../wrappers/viewmanager.h"
#include "../helpers/criticalSection.h"
#include "../helpers/physics.h"
#include <vector>
#include <cmath>

#define baseRadius 10
#define HOLDRANGE 300

magnetGun::magnetGun() : beamWidthFactor(1), theMouse(mouse::getInstance()),
mode(0), target(0), imgCenter(3,8)
{
	imgId = viewManager::getInstance()->loadTexture("resource/images/PS_arm.png");
	CRITICAL_INIT;
}

magnetGun::~magnetGun()
{
	CRITICAL_RELEASE;
}

void magnetGun::updateWorldMatrix()
{
	matrix combined;
	matrix transform;

	calc::matrixTranslate(transform, vector3((float)-imgCenter.x,(float)-imgCenter.y,0));
	combined *= transform;

	float ang = calc::angleBetweenVects(vector3(-1), aimVect);

	if(pos.y < theMouse->getPos().y)
		ang *= -1;

	if(pos.x < theMouse->getPos().x)
		ang -= PI;

	calc::matrixScale(transform, vector3((float)owner->getFacing(),1,1));
	combined *= transform;

	calc::matrixRotationZ(transform, ang);
	combined *= transform;

	calc::matrixTranslate(transform, pos);
	combined *= transform;

	CRITICAL(worldMatrix = combined);
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
		//fireLine = (theMouse->getPos() - pos).normalized();
		fireLine = aimVect;
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
	if(mode)
		mode = mode;

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

	if(mode != dat)
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

	//vector3 perp = (farPoint - pos).normalized();
	vector3 perp = vector3(aimVect.y, -aimVect.x);
	float centerLineProj = pos.dot2D(perp);
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
		std::vector<RECT>& magRect = CTileEngine::GetInstance()->GetMagnets();

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

void magnetGun::update(float dt)
{
	pos = owner->getPosition();
	pos.y -= 30;

	pos.x += owner->getFacing() * -4;

	if(target)
		aimVect = target->getPosition() - pos;
	else
		aimVect = theMouse->getPos() - pos;

	aimVect.normalize();

	if(aimVect.x > 0)
		owner->setFacing(FACE_LEFT);
	else
		owner->setFacing(FACE_RIGHT);

	updateWorldMatrix();

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
		//vector3 traj = (theMouse->getPos() - pos).normalized();

#define traj aimVect
		switch(mode)
		{
		case MAG_PUSH:
			if(target)
			{
				if((target->getPosition() - pos).length() < range)
					target->modPos(traj * (power * dt));
			}
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
	CRITICAL({
		viewManager::getInstance()->drawTexture(imgId, 0, &worldMatrix);
	});
}