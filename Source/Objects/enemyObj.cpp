#include "enemyObj.h"
#include <cmath>
#include "../helpers/datatypes.h"
#include "objManager.h"
#include "../Engines/CParticleEffectManager.h"
#include "../eventsystem/eventmanager.h"
#include "../eventsystem/eventids.h"
#include "..\States\GameStates\gamePlayState.h"

enemyObj::enemyObj(int type) : actorObj(OBJ_ENEMY), enemyType(type)
{
	m_bDied = false;
	m_nHP = 3;
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
			if(gamePlayState::getInstance()->GetIsGodMode() != true)
				EM->sendEvent(EVENT_PLAYERKILLED, obj);
			// Sets the bool to true so it will render the explosion
			//m_bDied = true;
		}
	}
	else if(obj->getType() == OBJ_MOVING && enemyType != ET_BOSS)
	{
		// Removes the enemy since it died
		EM->sendEvent(EVENT_ACTORDIED, this);
	}
	else if(enemyType == ET_SPIDER)
	{
		EM->sendEvent(EVENT_ACTORDIED, this);
	}
	if(enemyType == ET_BOSS && obj->getType() != OBJ_MOVING)
	{
		m_nHP-=1;
		if(m_nHP <= 0)
			EM->sendEvent(EVENT_ACTORDIED, this);
	}


	return holder.overlapped || holder.willCollide;
}