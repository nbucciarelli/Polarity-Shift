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
#include "CTileEngine.h"
#include "../States/GameStates/gamePlayState.h"
#include <cmath>

CAIEngine::CAIEngine() : trackPos(NULL)
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
		//if(player->getPosition().e[1] < (enemyList[j]->getPosition().e[1] + 50) && player->getPosition().e[1] > (enemyList[j]->getPosition().e[1] - 50))
		if(trackPos->y < (enemyList[j]->getPosition().e[1] + 50) && trackPos->y > (enemyList[j]->getPosition().e[1] - 50))
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
	for(unsigned int i = 0; i < CTileEngine::GetInstance()->GetTriggers().size(); i++)
	{
		if(trackPos->x > CTileEngine::GetInstance()->GetTriggers()[i].x)
		{
			gamePlayState::getInstance()->m_bTrapActive = true;
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
		trackPos = &((baseObj*)(ev->getData()))->getPosition();
		break;
	}
}