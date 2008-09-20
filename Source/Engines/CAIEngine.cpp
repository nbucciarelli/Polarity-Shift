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
#include"../Objects/CPowerUp.h"
#include"../EventSystem/eventManager.h"
#include "CTileEngine.h"
#include "../States/GameStates/gamePlayState.h"
#include "..\Helpers\bitFont.h"
#include "../EventSystem/playHandler.h"
#include <cmath>

CAIEngine::CAIEngine() : trackPos(NULL)
{
	player = NULL;
	movingLeft = true;
	movingDown = true;
	spawnTimer = 0.0f;
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

void CAIEngine::update(float dt)
{

	//loop through the enemies
	for(unsigned int j = 0; j < enemyList.size(); j++)
	{
		if(enemyList[j]->getEnemyType() == ET_SPIDER)
		{
			//check players y position
			//if(player->getPosition().e[1] < (enemyList[j]->getPosition().e[1] + 50) && player->getPosition().e[1] > (enemyList[j]->getPosition().e[1] - 50))
			if(trackPos->y < (enemyList[j]->getPosition().e[1] + 50) && trackPos->y > (enemyList[j]->getPosition().e[1] - 50))
			{
				//check players x position
				if(trackPos->x < enemyList[j]->getPosition().x)
				{
					enemyList[j]->modAcc(vector3(-10,0,0));
					if(enemyList[j]->getVelocity().x < -100)
						enemyList[j]->setVel(vector3(-100));
				}
				else if(trackPos->x > enemyList[j]->getPosition().x)
				{
					enemyList[j]->modAcc(vector3(10,0,0));
					if(enemyList[j]->getVelocity().x > 100)
						enemyList[j]->setVel(vector3(100));

				}
			}
			else
			{
				enemyList[j]->setAcc(vector3(0,0,0));
				enemyList[j]->setVel( vector3((float)(enemyList[j]->getVelocity().x * .99),enemyList[j]->getVelocity().y,0));
			}
		}
		else if(enemyList[j]->getEnemyType() == ET_BOSS)
		{
			spawnTimer = spawnTimer + dt;
			if(spawnTimer >= 10.0f)
			{
				eventManager::getInstance()->sendEvent(EVENT_TRAP_ACTIVE);
				eventManager::getInstance()->sendEvent(EVENT_DIDFALSE);
				spawnTimer = 0.0f;

			}
			if(enemyList[j]->getHP() == 3)
			{
				enemyList[j]->setVel(vector3(0,0,0));
				enemyList[j]->setPos(vector3(enemyList[j]->GetPosX(), enemyList[j]->GetPosY(),0));
			}
			else if(enemyList[j]->getHP() == 2)
			{
				enemyList[j]->setVel(vector3(0,0,0));
				if(enemyList[j]->GetPosX() > 124  && movingLeft == true)
				{
					enemyList[j]->setPos(vector3(enemyList[j]->GetPosX() - 100*dt,100,0));
					if(enemyList[j]->GetPosX() <= 124)
					{
						movingLeft = false;
					}
				}
				else if(enemyList[j]->GetPosX() < 900 && movingLeft == false)
				{
					enemyList[j]->setPos(vector3(enemyList[j]->GetPosX() + 100*dt,100,0));
					if(enemyList[j]->GetPosX() >= 900)
					{
						movingLeft = true;
					}
				}


			}
			else if(enemyList[j]->getHP() == 1)
			{
				//enemyList[j]->setVel(vector3(0,0,0));
				if(enemyList[j]->GetPosX() > 124  && movingLeft == true)
				{
					enemyList[j]->setPos(vector3(enemyList[j]->GetPosX() - 100*dt,enemyList[j]->GetPosY(),0));
					if(enemyList[j]->GetPosX() <= 124)
					{
						movingLeft = false;
					}
				}
				else if(enemyList[j]->GetPosX() < 900 && movingLeft == false)
				{
					enemyList[j]->setPos(vector3(enemyList[j]->GetPosX() + 100*dt,enemyList[j]->GetPosY(),0));
					if(enemyList[j]->GetPosX() >= 900)
					{
						movingLeft = true;
					}
				}
				if(enemyList[j]->GetPosY() < 550  && movingDown == true)
				{
					enemyList[j]->setVelY(100);
					//enemyList[j]->setPos(vector3(enemyList[j]->GetPosX(),enemyList[j]->GetPosY() + 100*dt,0));
					if(enemyList[j]->GetPosY() >= 550)
					{
						movingDown = false;
					}
				}
				else //if(enemyList[j]->GetPosY() > 100 && movingDown == false)
				{
					enemyList[j]->setVelY(0);
					enemyList[j]->setPos(vector3(enemyList[j]->GetPosX(),enemyList[j]->GetPosY() - 100,0));
					if(enemyList[j]->GetPosY() <= 100)
					{
						movingDown = true;
					}
				}
			}
		}
	}
	for(unsigned int i = 0; i < CTileEngine::GetInstance()->GetTriggers().size(); i++)
	{
		if(trackPos->x > CTileEngine::GetInstance()->GetTriggers()[i].x)
		{
			eventManager::getInstance()->sendEvent(EVENT_TRAP_ACTIVE);
		}
	}
	if(trackPos->y < (CTileEngine::GetInstance()->GetPlayerEnd().y + 25) && trackPos->y > (CTileEngine::GetInstance()->GetPlayerEnd().y - 25) && trackPos->x >= CTileEngine::GetInstance()->GetPlayerEnd().x)
	{
		eventManager::getInstance()->sendEvent(EVENT_LEVELFINISHED);
	}
	if(trackPos->y >= 650)
	{
		eventManager::getInstance()->sendEvent(EVENT_PLAYERKILLED);
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
	case EVENT_POWERUPLOAD:
		powerUpList.push_back((CPowerUp*)(ev->getData()));
		break;

	}
}
