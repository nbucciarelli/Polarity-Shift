////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “pauseState.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the pause state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "pauseState.h"
#include "mainMenuState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"

#pragma region constructor/destructor, singleton
pauseState::pauseState(void) : menuState(700, 400)
{
	menuItemString = new char*[TOTAL];
	menuLast = EXIT;

	menuItemString[RESUME] = "Resume";
	menuItemString[EXIT] = "Exit";
}

pauseState::~pauseState(void)
{
}

pauseState* pauseState::getInstance()
{
	static pauseState elevator;
	return &elevator;
}
#pragma endregion

#pragma region enter/exit
void pauseState::enter(void)
{
	menuState::enter();

	EM = eventManager::getInstance();
}
void pauseState::exit(void)
{
	menuState::exit();
}
#pragma endregion

#pragma region frame cycle
bool pauseState::input(float dt)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		EM->sendGlobalEvent(GE_STATE_POP);
	}

	return menuState::input(dt);
}

void pauseState::update(float dt)
{
	menuState::update(dt);
}

void pauseState::render(void) const
{
	menuState::render();
}
#pragma endregion

void pauseState::menuHandler()
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