#include "magnetGun.h"
#include "../helpers/datatypes.h"
#include "../helpers/physics.h"
#include "baseObj.h"
#include "objManager.h"
#include <vector>
#include <cmath>

#define baseRadius 10

magnetGun::magnetGun() : beamWidthFactor(1)
{
}

magnetGun::~magnetGun()
{
}

void magnetGun::fire(const vector3 &trajectory)
{
	//TODO:  IT!

	//Start by putting the range on the trajectory
	//Also, the perpendicular vector
	vector3 fireLine = trajectory.normalized();
	vector3 radiusLine(fireLine.y, -fireLine.x, 0);

	fireLine *= (float)range;
	radiusLine *= baseRadius * beamWidthFactor;
}

baseObj* magnetGun::getTarget(const vector3& farPoint)
{
	unsigned selection = -1;
	float minDist = (float)_HUGE, min = 0, max = 0, dist = 0;

	//Just to sneak in this variable.  Makes it look less scary in clas declaration.
	//And magical, too.
	static const std::vector<baseObj*>& objList = objManager::getInstance()->getList();

	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(owner == objList[c])
			continue;

		if(calc::lineIntersectPoly(owner->getPosition(), farPoint,
								*objList[c]->getCollisionPoly(), &dist)
			&& dist < minDist)
		{
			minDist = dist;
			selection = c;
		}
	}

	if(selection != -1)
		return objList[selection];
	else
		return 0;
}