////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “pauseState.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the pause state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "CCreditsState.h"
#include "mainMenuState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"

#pragma region constructor/destructor, singleton
CCreditsState::CCreditsState(void)
{
	menuItemString = new char*[TOTAL];
	menuLast = EXIT;

	menuItemString[RESUME] = "Resume";
	menuItemString[EXIT] = "Exit";
}

CCreditsState::~CCreditsState(void)
{
}

CCreditsState* CCreditsState::getInstance()
{
	static CCreditsState asdf;
	return &asdf;
}
#pragma endregion

#pragma region enter/exit
void CCreditsState::enter(void)
{
	menuState::enter();

	EM = eventManager::getInstance();
}
void CCreditsState::exit(void)
{
	menuState::exit();
}
#pragma endregion

#pragma region frame cycle
bool CCreditsState::input(float dt)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		EM->sendGlobalEvent(GE_STATE_POP);
	}

	return menuState::input(dt);
}

void CCreditsState::update(float delta)
{
	menuState::update(delta);
}

void CCreditsState::render(void) const
{
	menuState::render();
}
#pragma endregion

void CCreditsState::menuHandler()
{
	switch(menuPos)
	{
	case RESUME:
		EM->sendGlobalEvent(GE_STATE_POP);
		break;
	default:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
	}
}