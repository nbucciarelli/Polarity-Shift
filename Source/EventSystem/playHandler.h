//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"playerHandler.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	Handles the events for the player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "listener.h"

class eventManager;
class objManager;
class objFactory;
class objFileLoader;
class baseObj;

class playHandler : public listener
{
protected:
	eventManager* EM;
	objManager* OM;
	objFactory* OF;
	objFileLoader* FL;

	void onGameLoad();
	void killActor(baseObj* obj);

public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"HandleEvent"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Handles the event system for the player
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void HandleEvent(gameEvent* ev);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"initialize"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Initializes the event system for the player
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void initialize();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"shutdown"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Stops the event system for the player
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void shutdown();
};