#include "enemyObj.h"
#include <cmath>
#include "../helpers/datatypes.h"
#include "objManager.h"
#include "../Engines/CParticleEffectManager.h"
#include "../eventsystem/eventmanager.h"
#include "../eventsystem/eventids.h"

enemyObj::enemyObj(int type) : actorObj(OBJ_ENEMY), enemyType(type)
{
	m_bDied = false;
}

enemyObj::~enemyObj()
{}

void enemyObj::update(float dt)
{
	actorObj::update(dt);
}

void enemyObj::render()
{
		baseObj::render();
}

bool enemyObj::checkCollision(baseObj* obj, polyCollision* result)
{
	polyCollision holder;

	if(!actorObj::checkCollision(obj, &holder))
		return false;

	eventManager* EM = eventManager::getInstance();

	if(obj->getType() == OBJ_PLAYER)
	{
		if(enemyType == ET_SPIDER)
		{
			// Removes the spider enemy since it died
//			EM->sendEvent(EVENT_ACTORDIED, this);
			EM->sendEvent(EVENT_PLAYERKILLED, obj);
			// Sets the bool to true so it will render the explosion
			//m_bDied = true;
		}
	}
	else if(obj->getType() == OBJ_MOVING)
	{
		// Removes the enemy since it died
		EM->sendEvent(EVENT_ACTORDIED, this);
	}

	return holder.overlapped || holder.willCollide;
}