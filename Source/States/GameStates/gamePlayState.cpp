#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\eventIDs.h"
#include "../../EventSystem/playHandler.h"
#include "../../Objects/objManager.h"

gamePlayState::gamePlayState() {}
gamePlayState::~gamePlayState() {}

gamePlayState* gamePlayState::getInstance()
{
	static gamePlayState playa;

	return &playa;
}

void gamePlayState::enter(void)
{
	OM = objManager::getInstance();
	theInput = CSGD_DirectInput::GetInstance();
	EM = eventManager::getInstance();
	handler = new playHandler;

	handler->initialize();

	EM->sendEvent(EVENT_GAMELOADING);
}

void gamePlayState::exit(void)
{
	OM->clear();
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
	OM->update(dt);

	OM->checkCollisions();

	EM->processEvents();
}

void gamePlayState::render(void) const
{
	OM->render();
}