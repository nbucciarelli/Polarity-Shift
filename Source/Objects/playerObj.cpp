#include "playerObj.h"
#include "../EventSystem/gameEvent.h"
#include "../EventSystem/eventIDs.h"
#include <cmath>

playerObj::playerObj() : maxVel(0), maxAcc(0), range(0), jumpTime(0), maxJumpTime(0),
accStep(0)
{
}

playerObj::~playerObj()
{
}

void playerObj::update(float dt)
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
}

void playerObj::HandleEvent(gameEvent *ev)
{
	switch(ev->getEventID())
	{
	case EVENT_PLAYERGOLEFT:
		acceleration.x -= accStep;
		break;
	case EVENT_PLAYERGORIGHT:
		acceleration.x += accStep;
		break;
	case EVENT_PLAYERSTOP:
		acceleration.x = 0;
		velocity.x = 0;
		break;
	}
}