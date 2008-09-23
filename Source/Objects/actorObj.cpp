#include "actorObj.h"
#include "weapon.h"
#include "../EventSystem/eventIDs.h"
#include "../EventSystem/eventManager.h"
#include <cmath>

actorObj::actorObj(unsigned type) : movingObj(type), maxAcc(0), maxVel(200), accStep(0), theWeapon(0),
									health(100)
{
		for(int c = BEGIN_ACTOR_EVENTS + 1; c < END_ACTOR_EVENTS; c++)
			eventManager::getInstance()->registerClient(c, this);
}

actorObj::actorObj(const actorObj& obj) : movingObj(obj), maxAcc(obj.maxAcc),
				maxVel(obj.maxVel), accStep(obj.accStep), theWeapon(obj.theWeapon)
{
	if(obj.theWeapon)
		setWeapon(obj.theWeapon->getTypeID());
}

actorObj::~actorObj()
{
	eventManager::getInstance()->unregisterClient(this);

	if(theWeapon)
		theWeapon->release();
}

void actorObj::setWeapon(int weapID)
{
	theWeapon = weapon::createWeapon(weapID, this);
}

void actorObj::HandleEvent(gameEvent *ev)
{
	movingObj::HandleEvent(ev);
}

void actorObj::update(float dt)
{
	movingObj::update(dt);

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

	if(theWeapon)
		theWeapon->update(dt);
}