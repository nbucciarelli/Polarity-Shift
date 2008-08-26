#pragma once

#include "listener.h"

class eventManager;
class objManager;
class objFactory;

class playHandler : public listener
{
protected:
	eventManager* EM;
	objManager* OM;
	objFactory* OF;

	void onGameLoad();

public:
	void HandleEvent(gameEvent* ev);

	void initialize();
	void shutdown();
};