////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “playerObj.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the object that will be made into the player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "playerObj.h"
#include "../EventSystem/eventManager.h"
#include "../EventSystem/gameEvent.h"
#include "../EventSystem/eventIDs.h"
#include "../Engines/CAnimationManager.h"
#include "../Engines/CAnimationEngine.h"
#include "../Helpers/criticalSection.h"
#include "magnetGun.h"
#include <cmath>

#define JUMPS 2

//This can be a float.
#define JUMPPOWER 125

playerObj::playerObj() : actorObj(OBJ_PLAYER), jumpTime(0), maxJumpTime(0.5f), jumpCount(0),
						jumpDone(true)
{}

playerObj::~playerObj()
{
}

void playerObj::update(float dt)
{
	actorObj::update(dt);

	if(isOnSurface())
	{
		jumpCount = 0;
		jumpTime = 0;
	}
}

void playerObj::HandleEvent(gameEvent *ev)
{
	int a = 0;
	switch(ev->getEventID())
	{
	case EVENT_PLAYERGOLEFT:
		acceleration.x -= 50.0f;
		this->m_pAM->GetEngine(1)->Stop();
		if(GetAnimNumber() != 0)
		{
			this->m_pAM->GetEngine(0)->Play(true);
			this->SetAnimNumber(0);
		}
		this->setFacing(FACE_RIGHT);
		break;
	case EVENT_PLAYERGORIGHT:
		acceleration.x += 50.0f;
		this->m_pAM->GetEngine(1)->Stop();
		if(GetAnimNumber() != 0)
		{
			this->m_pAM->GetEngine(0)->Play(true);
			this->SetAnimNumber(0);
		}
		this->setFacing(FACE_LEFT);
		break;
	case EVENT_PLAYERSTOP:
		acceleration.x = 0;
		velocity.x = 0;
		this->m_pAM->GetEngine(0)->Stop();
		this->m_pAM->GetEngine(1)->Play(true);
		this->SetAnimNumber(1);
		break;
	case EVENT_PLAYERJUMP:
		if(jumpDone && jumpCount < JUMPS - 1)
		{
			jumpTime = 0;
			jumpCount++;
			jumpDone = false;
		}
		if(!jumpDone && jumpTime < maxJumpTime)
		{
			jumpTime+=frameTime;
			velocity.y = -JUMPPOWER;
		}
		break;
	case EVENT_PLAYERJUMPSTOP:
		jumpDone = true;
		break;
	case EVENT_PLAYERFIRE:
		theWeapon->openFire(0, MAG_PUSH);
		break;
	case EVENT_PLAYERFIRE2:
		theWeapon->openFire(0, MAG_PULL);
		break;
	case EVENT_PLAYERFIRE3:
		theWeapon->openFire(0, MAG_HOLD);
		break;
	case EVENT_PLAYERCEASEFIRE:
		if(ev->getData())
		{
			switch(*(int*)ev->getData())
			{
			case EVENT_PLAYERFIRE:
				a = MAG_PUSH;
				break;
			case EVENT_PLAYERFIRE2:
				a = MAG_PULL;
				break;
			default:
				a = 0;
			}
			delete ev->getData();
		}
		theWeapon->ceaseFire(a);
		break;
	case EVENT_PLAYERKILLED:
		eventManager::getInstance()->sendEvent(EVENT_PLAYER_DIED);
		break;
	default:
		actorObj::HandleEvent(ev);
	}
}