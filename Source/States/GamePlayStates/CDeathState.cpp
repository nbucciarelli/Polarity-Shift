/////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CDeathState.cpp"
//
// Authors: Nicholas Bucciarelli (NB)
// 
// Purpose: The death state
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CDeathState.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\viewManager.h"
#include "..\..\Wrappers\CSGD_FModManager.h"
#include "..\..\game.h"

CDeathState::CDeathState()
{
	menuItemString = new char*[TOTAL];

	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

CDeathState::~CDeathState()
{

}

void CDeathState::enter(void)
{
	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound1()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound1());

	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound2()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound2());
	menuState::enter();
	m_nBackGroundImageID = viewManager::getInstance()->loadTexture("Resource/PS_death.png");
}

void CDeathState::exit(void)
{
	menuState::exit();
	viewManager::getInstance()->releaseTexture(m_nBackGroundImageID);
	
}

void CDeathState::update(float dt)
{
	
}

void CDeathState::render(void) const
{
	viewManager::getInstance()->drawTexture(m_nBackGroundImageID, &vector3());
}

void CDeathState::menuHandler()
{
	switch(menuPos)
	{
	case EXIT:
		//EM->sendGlobalEvent(GE_STATE_POP);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_LEVELSELECT));
		break;
	}
}