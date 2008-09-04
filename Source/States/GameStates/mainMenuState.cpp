//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"mainManuState.cpp"
//	Author:			Scott Smallback (SS) / Nick Bucciarelli (NB)
//	Purpose:		handles the main menus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mainMenuState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Engines\CParticleEffectManager.h"

mainMenuState::mainMenuState(void)
{
	menuItemString = new char*[TOTAL];

	menuItemString[PLAY] = "Play";
	menuItemString[OPTIONS] = "Options";
	menuItemString[HOWTO] = "How to play";
	menuItemString[SCORES] = "High Scores";
	menuItemString[CREDITS] = "Credits";
	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

mainMenuState::~mainMenuState(void)
{
}

mainMenuState* mainMenuState::getInstance()
{
	static mainMenuState Elgoog;
	return &Elgoog;
}

void mainMenuState::enter(void)
{

	m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_CursorParticle.prt");
	CParticleEffectManager::GetInstance()->Play(m_nParticleImageID, true);
	menuState::enter();

}

void mainMenuState::exit(void)
{
	menuState::exit();
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
	CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
}
void mainMenuState::update(float dt)
{
	if(!entered)
		return;

	CParticleEffectManager::GetInstance()->Update(dt);
}

void mainMenuState::menuHandler()
{
	switch(menuPos)
	{
	case PLAY:
		//game::GetInstance()->changeState(playState::getInstance());
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case OPTIONS:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_OPTIONS));
		break;
	case HOWTO:
		break;
	case SCORES:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_HIGHSCORES));
		break;
	case CREDITS:
		break;
	default:
		EM->sendGlobalEvent(GE_STATE_CLEARALL);
	}
}

void mainMenuState::render(void) const
{
	if(!entered)
		return;

	menuState::render();
	CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetXPos(), menuState::GetYPos()+ 10 + menuState::GetMenuPos() * 50); 
	
}