////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “playerObj.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the object that will be made into the player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "playerObj.h"
#include "../EventSystem/gameEvent.h"
#include "../EventSystem/eventIDs.h"
#include "../EventSystem/eventManager.h"
//#include "../Engines/CAnimationManager.h"
//#include "../Engines/CAnimationEngine.h"
#include <cmath>

playerObj::playerObj() : maxVel(50), maxAcc(0), range(0), jumpTime(0), maxJumpTime(0.2f),
accStep(0), jumpDone(false)
{
//	m_pAM = CAnimationManager::GetInstance();
	// THIS WOULD BE FOR THE PLAYER AND HIS LOADING OF TEH ACTUAL BINARY INFORMATION
	//m_pAM->Load("C:/Documents and Settings/Administrator/My Documents/ironman.anm", this);
	//SetAnimNumber(1);
}

playerObj::~playerObj()
{
	eventManager::getInstance()->unregisterClient(this);
}

void playerObj::update(float dt)
{
	movingObj::update(dt);
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
/*void playerObj::render()
{
	/*if(GetAnimNumber() == 0)
	{
		SetWidth(m_pAM->GetEngine(0)->GetCurrentFrame()->GetWidth());
		SetHeight(m_pAM->GetEngine(0)->GetCurrentFrame()->GetHeight());

		if (GetIsFlipped() == false)
			m_pAM->Render(0, (int)GetPosX(), (int)GetPosY());
		else
			m_pAM->Render(0, (int)GetPosX() + GetWidth(), (int)GetPosY(), -1);
	}
	else if(GetAnimNumber() == 1)
	{
		SetWidth(m_pAM->GetEngine(1)->GetCurrentFrame()->GetWidth());
		SetHeight(m_pAM->GetEngine(1)->GetCurrentFrame()->GetHeight());

		if (GetIsFlipped() == false)
			m_pAM->Render(1, (int)GetPosX(), (int)GetPosY());
		else
			m_pAM->Render(1, (int)GetPosX() + GetWidth(), (int)GetPosY(), -1);
	}*/
/*	int num = GetAnimNumber();

	setDimensions(m_pAM->GetEngine(num)->GetCurrentFrame()->GetWidth(),
				m_pAM->GetEngine(num)->GetCurrentFrame()->GetHeight());
	//setWidth(m_pAM->GetEngine(num)->GetCurrentFrame()->GetWidth());
	//setHeight(m_pAM->GetEngine(num)->GetCurrentFrame()->GetHeight());

	if (getFacing() == FACE_LEFT)
		m_pAM->Render(num, (int)getPosition().x, (int)getPosition().y);
	else
		m_pAM->Render(num, (int)getPosition().x + getDimensions().x, (int)getPosition().y, -1);
}*/

void playerObj::HandleEvent(gameEvent *ev)
{
	switch(ev->getEventID())
	{
	case EVENT_PLAYERGOLEFT:
		acceleration.x -= 100.0f;
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER MOVING LEFT FOR INFORMATION
		//m_pPlayer->m_pAM->GetEngine(1)->Stop();
		//if(m_bIsRunningAnim == false)
		//{
		//	m_bIsRunningAnim = true;
		//	m_pPlayer->m_pAM->GetEngine(0)->Play(true);
		//}
		//m_pPlayer->SetAnimNumber(0);
		//m_pES->SendEvent("MoveRight");
		//m_pPlayer->SetIsFlipped(false);
		break;
	case EVENT_PLAYERGORIGHT:
		acceleration.x += 100.0f;
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER MOVING RIGHT FOR INFORMATION
		//m_pPlayer->m_pAM->GetEngine(1)->Stop();
		//if(m_bIsRunningAnim == false)
		//{
		//	m_bIsRunningAnim = true;
		//	m_pPlayer->m_pAM->GetEngine(0)->Play(true);
		//}
		//m_pPlayer->SetAnimNumber(0);
		//m_pES->SendEvent("MoveRight");
		//m_pPlayer->SetIsFlipped(false);
		break;
	case EVENT_PLAYERSTOP:
		acceleration.x = 0;
		velocity.x = 0;
		// THIS CODE NEEDS TO BE SUITED FOR THE ACTUAL PLAYER STOPPING FOR INFORMATION
		//m_bIsRunningAnim = false;
		//m_pPlayer->m_pAM->GetEngine(0)->Stop();
		//m_pPlayer->m_pAM->GetEngine(1)->Play(true);
		//m_pPlayer->SetAnimNumber(1);
		//m_pES->SendEvent("StopX");
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