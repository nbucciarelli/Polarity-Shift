////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “playerObj.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the object that will be made into the player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "playerObj.h"
#include "../EventSystem/gameEvent.h"
#include "../EventSystem/eventIDs.h"
#include "../EventSystem/eventManager.h"
#include "../Engines/CAnimationManager.h"
#include "../Engines/CAnimationEngine.h"
#include "../Helpers/criticalSection.h"
#include <cmath>

playerObj::playerObj() : maxVel(200), maxAcc(0), range(0), jumpTime(0), maxJumpTime(0.2f),
accStep(0), jumpDone(false), movingObj(OBJ_PLAYER)
{
	eventManager::getInstance()->sendEvent(EVENT_PLAYERLOAD, this);
	m_pAM = CAnimationManager::GetInstance();
	// THIS WOULD BE FOR THE PLAYER AND HIS LOADING OF TEH ACTUAL BINARY INFORMATION
	m_pAM->Load("Resource/PS_ironman.anm", this);
	SetAnimNumber(1);
}

playerObj::~playerObj()
{
	eventManager::getInstance()->unregisterClient(this);
}

void playerObj::update(float dt)
{
	movingObj::update(dt);
	m_pAM->Update(dt);
	// IF ANIMATION IS RENDERING, NEEDS TO UPDATE THE ANIMATION MANAGER FOR ACTUAL ANIMATION

	if(maxAcc && fabs(acceleration.x) > maxAcc)
	{
		if(acceleration.x < 0)
			acceleration.x = -maxAcc;
		else
			acceleration.x = maxAcc;
	}

	if(maxVel && fabs(velocity.x) > maxVel)
	{
		if(velocity.x < 0)
			velocity.x = -maxVel;
		else
			velocity.x = maxVel;

		acceleration.x = 0;
	}

	//Should be "isOnGround" or some such.
	if(jumpTime && position.y == 500)
	{
		jumpTime = 0;
	}
}

//THIS NEEDS TO BE CODED INTO THE GAME FOR THE PLAYER WHEN THE PLAYER IS ACTUALLY ANIMATING (NEEDS TO RENDER HIS OWN)
void playerObj::render()
{
	int num = GetAnimNumber();

	setDimensions(m_pAM->GetEngine(num)->GetCurrentFrame()->GetWidth(),
				m_pAM->GetEngine(num)->GetCurrentFrame()->GetHeight());

	CRITICAL(m_pAM->Render(num, &worldMatrix));
	//if(getFacing() == FACE_RIGHT)
	//{
	//	CRITICAL(m_pAM->Render(num, (int)getPosition().x, (int)getPosition().y));
	//}
	//else
	//{
	//	CRITICAL(m_pAM->Render(num, (int)getPosition().x + (int)getDimensions().x, (int)getPosition().y, -1));
	//}
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
		if(jumpTime < 1)
		{
			jumpTime++;
			velocity.y = -250;
		}
		break;
	case EVENT_PLAYERJUMPSTOP:
		jumpDone = true;
		break;
	}
}