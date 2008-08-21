#include "globalHandler.h"

#include "eventManager.h"
#include "gameEvent.h"
#include "globalEvents.h"

#include "../game.h"

#include "../states/gamestates/gamestateslist.h"

#define STARTSTATE STATE_MAINMENU

#pragma region constructor, destructor, singleton
globalHandler::globalHandler() {}
globalHandler::~globalHandler() {}

globalHandler* globalHandler::getInstance()
{
	static globalHandler Aeon;
	return &Aeon;
}

#pragma endregion

#pragma region init/shutdown
void globalHandler::initialize()
{
	EM = eventManager::getInstance();
	theGame = game::GetInstance();

	//Register for global events.
	for(int c = GE_NULL + 1; c < GE_MAX; c++)
		EM->registerClient(c, this);
}

void globalHandler::shutdown()
{
	//Shutting down, clear out from EM.
	EM->unregisterClient(this);
}
#pragma endregion

void globalHandler::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case GE_STATE_CHANGETO:
		theGame->changeState(state(*(int*)ev->getData()));
		break;
	case GE_STATE_PUSH:
		theGame->pushState(state(*(int*)ev->getData()));
		break;
	case GE_STATE_POP:
		theGame->popState();
		break;
	case GE_STATE_CLEARALL:
		theGame->clearStates();
		break;
	case GE_GAME_START:  //This is different from change state in case anything else
						 //Needs adding.
		theGame->changeState(state(STARTSTATE));
		break;
	}
}

gameState* globalHandler::state(int st)
{
	switch(st)
	{
	case STATE_MAINMENU:
		return mainMenuState::getInstance();
		break;
	case STATE_PAUSE:
		//return pauseState::getInstance();
		//break;
	case STATE_PLAY:
		//return playState::getInstance();
		//break;
	default:
		return NULL;
	}
}