////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "CAIEngine.h"
// Author: Leland Nyman (LN)
// Purpose: This file handles all events associated with AI and updates them accordingly. 
// It also stores all information needed for the AI objects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CAIEngine.h"
#include "../EventSystem/eventIDs.h"
#include"../EventSystem/gameEvent.h"
#include"../Objects/enemyObj.h"
#include"../Objects/playerObj.h"
#include"../EventSystem/eventManager.h"
#include <cmath>

CAIEngine::CAIEngine()
{
	player = NULL;
	//register events
	for(int c = BEGIN_AI_EVENTS; c < END_AI_EVENTS; c++)
		eventManager::getInstance()->registerClient(c, this);
}

CAIEngine::~CAIEngine()
{
	//unregister events
	for(int c = BEGIN_AI_EVENTS; c < END_AI_EVENTS; c++)
		eventManager::getInstance()->unregisterClient(c, this);

}

void CAIEngine::update()
{
	//loop through the enemies
	for(unsigned int j = 0; j < enemyList.size(); j++)
	{
		//check players y position
		if(player->getPosition().y < (enemyList[j]->getPosition().y + 25) && player->getPosition().y > (enemyList[j]->getPosition().y - 25))
		{
			//check players x position
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
			//make enemy stop when player is not visible
			if(fabs(enemyList[j]->getVelocity().x) < 10)
				enemyList[j]->setVelX(0);
			enemyList[j]->setVel( vector3((float)(enemyList[j]->getVelocity().x * .99),enemyList[j]->getVelocity().y,0));
		}
	}

}

void CAIEngine::HandleEvent(gameEvent *ev)
{
	switch(ev->getEventID())
	{
	//adds all new enemies to the list
	case EVENT_ENEMYLOAD:
		enemyList.push_back((enemyObj*)(ev->getData()));
		break;
	//adds the player to the AI engine
	case EVENT_PLAYERLOAD:
		player = (playerObj*)(ev->getData());
		break;
	}
}