#include "gamePlayState.h"

#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"

gamePlayState::gamePlayState() {}
gamePlayState::~gamePlayState() {}

gamePlayState* gamePlayState::getInstance()
{
	static gamePlayState playa;

	return &playa;
}

void gamePlayState::enter(void)
{
	//objManager * objM;
	//inputDevice* theInput;
	//eventManager* EM;
	//eventHandler* EH;
}

void gamePlayState::exit(void)
{
}

bool gamePlayState::input(float dt)
{
	return true;
}

void gamePlayState::update(float dt)
{
}

void gamePlayState::render(void) const
{
}