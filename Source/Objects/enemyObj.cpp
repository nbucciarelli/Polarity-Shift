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
	m_nExplosionID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_Explosion.prt");
}

enemyObj::~enemyObj()
{}

void enemyObj::update(float dt)
{
	actorObj::update(dt);
}

void enemyObj::render()
{
	if(m_bDied == false)
	{
		baseObj::render();
	}
	else if(m_bDied == true)
	{
		CParticleEffectManager::GetInstance()->Play(m_nExplosionID, false);
	}
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
			// Saves the position of the spider (since it blows up) so it won't crash
			m_nExpX = this->position.x;
			m_nExpY = this->position.y;
			// Removes the spider enemy since it died
			EM->sendEvent(EVENT_ACTORDIED, this);
			EM->sendEvent(EVENT_PLAYERKILLED, obj);
			// Sets the bool to true so it will render the explosion
			//m_bDied = true;
		}
	}
	else if(obj->getType() == OBJ_MOVING)
	{
		// Saves the position of the enemy so it won't crash
		m_nExpX = this->position.x;
		m_nExpY = this->position.y;
		// Removes the enemy since it died
		EM->sendEvent(EVENT_ACTORDIED, this);
		// Sets the bool to true so it will render the explosion
		//m_bDied = true;
	}

	return holder.overlapped || holder.willCollide;
}