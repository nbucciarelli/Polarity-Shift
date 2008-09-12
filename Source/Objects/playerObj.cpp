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
#include "magnetGun.h"
#include <cmath>

playerObj::playerObj() : maxVel(200), maxAcc(0), range(0), jumpTime(0), maxJumpTime(0.2f),
accStep(0), jumpDone(false), theWeapon(0), weaponType(0), movingObj(OBJ_PLAYER)
{
	//eventManager::getInstance()->sendEvent(EVENT_PLAYERLOAD, this);
	m_pAM = CAnimationManager::GetInstance();
	// THIS WOULD BE FOR THE PLAYER AND HIS LOADING OF TEH ACTUAL BINARY INFORMATION
	m_pAM->Load("Resource/PS_ironman.anm", this);
	SetAnimNumber(1);
}

playerObj::playerObj(const playerObj& obj)
{
	memcpy_s(this, sizeof(playerObj), &obj, sizeof(playerObj));

	if(theWeapon)
	{
		theWeapon = weapon::createWeapon(weaponType, this);
	}
}

playerObj::~playerObj()
{
	eventManager::getInstance()->unregisterClient(this);

	m_pAM->Shutdown();
	if(theWeapon)
		delete theWeapon;
}

void playerObj::update(float dt)
{
	m_pAM->Update(dt);

	int num = GetAnimNumber();

	setDimensions(m_pAM->GetEngine(num)->GetCurrentFrame()->GetWidth(),
				m_pAM->GetEngine(num)->GetCurrentFrame()->GetHeight());

	imgCenter = *(pt*)(&m_pAM->GetEngine(num)->GetCurrentFrame()->pAnchor);

	//rect a = *(rect*)&m_pAM->GetEngine(num)->GetCurrentFrame()->rSource;

	movingObj::update(dt);

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
	if(jumpTime && onSurface)
	{
		jumpTime = 0;
	}

	if(theWeapon)
		theWeapon->update(dt);
}

//THIS NEEDS TO BE CODED INTO THE GAME FOR THE PLAYER WHEN THE PLAYER IS ACTUALLY ANIMATING (NEEDS TO RENDER HIS OWN)
void playerObj::render()
{
	int num = GetAnimNumber();

	CRITICAL(m_pAM->Render(num, &worldMatrix));
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
			onSurface = false;
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
	}
}

void playerObj::setWeapon(int weapID)
{
	theWeapon = weapon::createWeapon(weapID, this);
}

rect playerObj::getCollisionRect() const
{
	//rect val = *(rect*)&m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->rCollision;
	rect val = *(rect*)&m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->rSource;

	val.bottom = val.bottom - val.top;
	val.right = val.right - val.left;
	val.left = val.top = 0;

	val.top += (int)position.y;
	val.bottom += (int)position.y;
	val.left = (int)(val.left * scale.x + position.x);
	val.right = (int)(val.right * scale.x + position.x);

	if(val.left > val.right)
	{
		int holder = val.left;
		val.left = val.right;
		val.right = holder;
	}

	return val;
}