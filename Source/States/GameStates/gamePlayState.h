#pragma once
#include "../gameState.h"

class objManager;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class eventManager;
class playHandler;

class gamePlayState : public gameState
{
protected:
	objManager * objM;
	inputDevice* theInput;
	eventManager* EM;
	playHandler* handler;
	

	gamePlayState();
	gamePlayState(const gamePlayState&);
	gamePlayState& operator=(const gamePlayState&);
	~gamePlayState();

public:
	static gamePlayState* getInstance();

	virtual void enter(void);
	virtual void exit(void);

	virtual bool input(float dt);
	virtual void update(float dt);
	virtual void render(void) const;

};