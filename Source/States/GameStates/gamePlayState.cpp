#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
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
	theInput = CSGD_DirectInput::GetInstance();
	EM = eventManager::getInstance();
	//eventHandler* EH;
}

void gamePlayState::exit(void)
{
}

bool gamePlayState::input(float dt)
{
	if (theInput->KeyPressed(DIK_ESCAPE) || theInput->KeyPressed(DIK_F10))
		EM->sendGlobalEvent(GE_STATE_PUSH, new int(STATE_PAUSE));

	return true;
}

void gamePlayState::update(float dt)
{
}

void gamePlayState::render(void) const
{
}