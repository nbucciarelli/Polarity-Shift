#include "CHighScoresState.h"
#include "menustate.h"

#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"

CHighScoresState::CHighScoresState()
{
	menuItemString = new char*[TOTAL];
	menuLast = EXIT;

	menuItemString[EXIT] = "Exit";
}

bool CHighScoresState::CheckScores()
{
	return true;
}

bool CHighScoresState::input(float delta)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		EM->sendGlobalEvent(GE_STATE_POP);
	}
		
	return menuState::input(delta);
	
}

void CHighScoresState::render() const
{
	menuState::render();
}

void CHighScoresState::enter()
{
	menuState::enter();
	EM = eventManager::getInstance();

}

void CHighScoresState::exit()
{
	menuState::exit();

}

void CHighScoresState::update(float delta)
{
	
}

void CHighScoresState::menuHandler(void)
{

	switch(menuPos)
	{
	case EXIT:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
		break;
	}
}