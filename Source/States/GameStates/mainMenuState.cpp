#include "mainMenuState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"

mainMenuState::mainMenuState(void)
{
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
	menuState::enter();

	menuItemString = new char*[TOTAL];

	menuItemString[PLAY] = "Play";
	menuItemString[OPTIONS] = "Options";
	menuItemString[HOWTO] = "How to play";
	menuItemString[SCORES] = "High Scores";
	menuItemString[CREDITS] = "Credits";
	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

void mainMenuState::exit(void)
{
	delete[] menuItemString;
	menuState::exit();
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
	case HOWTO:
		break;
	case SCORES:
	case CREDITS:
		break;
	default:
		EM->sendGlobalEvent(GE_STATE_CLEARALL);
	}
}

void mainMenuState::render(void) const
{
	menuState::render();
}