#pragma once
#include "listener.h"

class eventManager;
class game;
class gameState;

class globalHandler : public listener
{
protected:
	eventManager* EM;
	game* theGame;

	gameState* state(int);

	globalHandler();
	globalHandler(const globalHandler&);
	globalHandler& operator=(const globalHandler&);
	~globalHandler();
public:
	static globalHandler* getInstance();

	void initialize();
	void shutdown();

	void HandleEvent(gameEvent* ev);
};