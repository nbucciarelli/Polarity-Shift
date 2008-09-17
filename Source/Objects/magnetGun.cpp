#include "magnetGun.h"
#include "../helpers/datatypes.h"
#include "../helpers/physics.h"
#include "playerObj.h"
#include "objManager.h"
#include "../wrappers/mouse.h"
#include <vector>
#include <cmath>

#define baseRadius 10

magnetGun::magnetGun() : beamWidthFactor(1), theMouse(mouse::getInstance()),
mode(0), target(0)
{
}

magnetGun::~magnetGun()
{
}

void magnetGun::openFire(const vector3 *trajectory, int fireMode)
{
	//If already firing, do nothing.
	if(isActive)
		return;

	//Start by putting the range on the trajectory
	//Also, the perpendicular vector
	vector3 fireLine;
	
	if(trajectory)
		fireLine = trajectory->normalized();
	//vector3 radiusLine(fireLine.y, -fireLine.x, 0);
	else
	{
		fireLine = (theMouse->getPos() - owner->getPosition()).normalized();
	}

	fireLine *= (float)range;
	//radiusLine *= baseRadius * beamWidthFactor;

	isActive = getTarget(fireLine + owner->getPosition());

	if(isActive && target)
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
		target->setAcc(vector3());
		target->setVel(vector3());
		break;
	default:
		break;
	}
}

void magnetGun::ceaseFire()
{
	if(!isActive)
		return;

	target = NULL;
	mode = MAG_OFF;
	isActive = false;
}

bool magnetGun::getTarget(const vector3& farPoint)
{
	int selection = -1;
	float minDist = (float)_HUGE, min = 0, max = 0, dist = 0;

	vector3 perp = (farPoint - owner->getPosition()).normalized();
	perp = vector3(perp.y, -perp.x);
	float centerLineProj = owner->getPosition() * perp;
	float radius = baseRadius * beamWidthFactor;

	//Just to sneak in this variable.  Makes it look less scary in class declaration.
	//And magical, too.
	static const std::vector<baseObj*>& objList = objManager::getInstance()->getList();

	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(owner == objList[c])
			continue;

		if(objList[c]->IsMovable()
			&&  calc::lineIntersectPoly(owner->getPosition(), farPoint,
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

	if(selection != -1 && minDist < range)
	{
		target = (movingObj*)(objList[selection]);
		return true;
	}
	else
		return false;
}

void magnetGun::update(float dt)
{
	if(mode != MAG_OFF)
	{
		target->setVel(vector3());
		target->setAcc(vector3());

		vector3 traj = (theMouse->getPos() - owner->getPosition()).normalized();

		switch(mode)
		{
		case MAG_PUSH:
			target->modPos(traj * (power * dt));
			break;
		case MAG_PULL:
			target->modPos(traj * -(power * dt));
			break;
		default:
			break;
		}
	}
}

void magnetGun::render() const
{
}
