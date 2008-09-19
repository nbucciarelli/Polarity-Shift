#include "enemyObj.h"
#include <cmath>
#include "../helpers/datatypes.h"
#include "objManager.h"
#include "../Engines/CParticleEffectManager.h"

enemyObj::enemyObj() : actorObj(OBJ_ENEMY)
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

	actorObj::checkCollision(obj, &holder);

	if(obj->getType() == OBJ_PLAYER)
	{
		if(enemyType == ET_SPIDER)
		{
			m_nExpX = this->position.x;
			m_nExpY = this->position.y;
			objManager::getInstance()->removeObj(this);
		}
	}
	else if(obj->getType() == OBJ_MOVING)
	{
		m_nExpX = this->position.x;
		m_nExpY = this->position.y;
		objManager::getInstance()->removeObj(this);
		m_bDied = true;
	}

	return holder.overlapped || holder.willCollide;
}