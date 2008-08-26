#include "playHandler.h"
#include "../Objects/objFactory.h"
#include "../Objects/objManager.h"
#include "eventManager.h"
#include "eventIDs.h"
#include "globalEvents.h"

void playHandler::initialize()
{
	EM = eventManager::getInstance();
	OM = objManager::getInstance();
	OF = objFactory::getInstance();

	for(int c = BEGIN_HANDLER_EVENTS + 1; c < END_HANDLER_EVENTS; c++)
	{
		EM->registerClient(c, this);
	}
}

void playHandler::shutdown()
{
	for(int c = BEGIN_HANDLER_EVENTS + 1; c < END_HANDLER_EVENTS; c++)
	{
		EM->unregisterClient(c, this);
	}
}


void playHandler::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case EVENT_GAMELOADING:
		onGameLoad();
		break;
	case EVENT_GAMEPAUSE:
		EM->sendGlobalEvent(GE_STATE_PUSH, new int(STATE_PAUSE));
		break;
	default:
		break;
	}
}

void playHandler::onGameLoad()
{
	
}