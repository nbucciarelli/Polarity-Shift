#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\eventIDs.h"
#include "../../EventSystem/playHandler.h"

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
	handler = new playHandler;

	handler->initialize();
}

void gamePlayState::exit(void)
{
	handler->shutdown();
	delete handler;
}

bool gamePlayState::input(float dt)
{
	if (theInput->KeyPressed(DIK_ESCAPE) || theInput->KeyPressed(DIK_F10))
		EM->sendEvent(EVENT_GAMEPAUSE);

	return true;
}

void gamePlayState::update(float dt)
{

	EM->processEvents();
}

void gamePlayState::render(void) const
{
}