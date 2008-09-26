/////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CWinState.cpp"
//
// Authors: Nicholas Bucciarelli (NB)
// 
// Purpose: The death state
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CWinState.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\viewManager.h"
#include "..\..\Wrappers\CSGD_FModManager.h"
#include "..\..\game.h"
#include "..\GameStates\CHighScoresState.h"

CWinState::CWinState()
{
	menuItemString = new char*[TOTAL];

	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

CWinState::~CWinState()
{

}

void CWinState::enter(void)
{
	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound1()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound1());

	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound2()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound2());
 
	menuState::enter();
	m_nBackGroundImageID = viewManager::getInstance()->loadTexture("Resource/PS_WinScreen.png");
	
	float fScores = 0;
	for (int i = 0 ; i < NUMLEVELS ; ++i)
		fScores += game::GetInstance()->GetHighScore(i);

	CHighScoresState::AddHighScore((int)fScores);
}

void CWinState::exit(void)
{
	menuState::exit();
	viewManager::getInstance()->releaseTexture(m_nBackGroundImageID);

}

void CWinState::update(float dt)
{

}

void CWinState::render(void) const
{
	viewManager::getInstance()->drawTexture(m_nBackGroundImageID, &vector3());
}

void CWinState::menuHandler()
{
	switch(menuPos)
	{
	case EXIT:
		//EM->sendGlobalEvent(GE_STATE_POP);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
		break;
	}
}