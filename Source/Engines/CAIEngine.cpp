#include "CAIEngine.h"
#include "../EventSystem/eventIDs.h"
#include"../EventSystem/gameEvent.h"
#include"../Objects/enemyObj.h"
#include"../Objects/playerObj.h"
#include"../EventSystem/eventManager.h"

CAIEngine::CAIEngine()
{

	player = NULL;
	EM = eventManager::getInstance();

	for(int c = BEGIN_AI_EVENTS; c < END_AI_EVENTS; c++)
		EM->registerClient(c, this);

}

CAIEngine::~CAIEngine()
{
	for(int c = BEGIN_AI_EVENTS; c < END_AI_EVENTS; c++)
		EM->unregisterClient(c, this);
}

void CAIEngine::update()
{

	for(unsigned int j = 0; j < enemyList.size(); j++)
	{
		if(player->getPosition().y < (enemyList[j]->getPosition().y + 25) && player->getPosition().y > (enemyList[j]->getPosition().y - 25))
		{
			if(player->getPosition().x < enemyList[j]->getPosition().x)
			{
				enemyList[j]->modAcc(vector3(-10,0,0));
			}
			else if(player->getPosition().x > enemyList[j]->getPosition().x)
			{
				enemyList[j]->modAcc(vector3(10,0,0));
			}
		}
		else
		{
			enemyList[j]->setVel( vector3((float)(enemyList[j]->getVelocity().x * .99),enemyList[j]->getVelocity().y,0));
		}
	}

}

void CAIEngine::HandleEvent(gameEvent *ev)
{
	switch(ev->getEventID())
	{
	case EVENT_ENEMYLOAD:
		enemyList.push_back((enemyObj*)(ev->getData()));
		break;
	case EVENT_PLAYERLOAD:
		player = (playerObj*)(ev->getData());
		break;
	}
}