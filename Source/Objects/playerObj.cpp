////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “playerObj.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the object that will be made into the player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "playerObj.h"
#include "../EventSystem/gameEvent.h"
#include "../EventSystem/eventIDs.h"
#include "../Engines/CAnimationManager.h"
#include "../Engines/CAnimationEngine.h"
#include "../Helpers/criticalSection.h"
#include "magnetGun.h"
#include <cmath>

playerObj::playerObj() : jumpTime(0), maxJumpTime(0.2f), jumpCount(0), jumpDone(false),
						 actorObj(OBJ_PLAYER)
{}

playerObj::~playerObj()
{
}

void playerObj::update(float dt)
{
	actorObj::update(dt);

	//Should be "isOnGround" or some such.
	if(jumpTime && onSurface)
	{
		jumpTime = 0;
	}
}

void playerObj::HandleEvent(gameEvent *ev)
{
	switch(ev->getEventID())
	{
	case EVENT_PLAYERGOLEFT:
		acceleration.x -= 100.0f;
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER MOVING LEFT FOR INFORMATION
		this->m_pAM->GetEngine(1)->Stop();
		if(GetAnimNumber() != 0)
		{
			this->m_pAM->GetEngine(0)->Play(true);
			this->SetAnimNumber(0);
		}
		this->setFacing(FACE_RIGHT);
		break;
	case EVENT_PLAYERGORIGHT:
		acceleration.x += 100.0f;
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER MOVING RIGHT FOR INFORMATION
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
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER STOPPING FOR INFORMATION
		this->m_pAM->GetEngine(0)->Stop();
		this->m_pAM->GetEngine(1)->Play(true);
		this->SetAnimNumber(1);
		break;
	case EVENT_PLAYERJUMP:
		if(true)
		{
			jumpTime++;
			velocity.y = -250;
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
		theWeapon->ceaseFire();
		break;
	default:
		actorObj::HandleEvent(ev);
	}
}