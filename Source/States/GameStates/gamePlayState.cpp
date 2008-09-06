////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “gamePlayState.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the game play state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\eventIDs.h"
#include "../../EventSystem/playHandler.h"
#include "../../Objects/objManager.h"
#include "..\..\Engines\CTileEngine.h"


gamePlayState::gamePlayState() : rendering(false) {}
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
	TE = new CTileEngine();
	TE->LoadMap("Resource/PS_TestLevel.bmf");

	gameState::enter();

}

void gamePlayState::exit(void)
{
	while(rendering)
		Sleep(1);

	gameState::exit();
	
	delete TE;
	delete AIE;
	OM->clear();
	handler->shutdown();
	delete handler;
}

bool gamePlayState::input(float dt)
{
	if(!entered)
		return true;

	if (theInput->KeyPressed(DIK_ESCAPE) || theInput->KeyPressed(DIK_F10))
		EM->sendEvent(EVENT_GAMEPAUSE);

	if(theInput->KeyDown(DIK_A))
		EM->sendEvent(EVENT_PLAYERGOLEFT);
	else if(theInput->KeyDown(DIK_D))
		EM->sendEvent(EVENT_PLAYERGORIGHT);
	else if(theInput->KeyReleased(DIK_A) || theInput->KeyReleased(DIK_D))
		EM->sendEvent(EVENT_PLAYERSTOP);

	if(theInput->KeyPressed(DIK_W))
		EM->sendEvent(EVENT_PLAYERJUMP);

	return true;
}

void gamePlayState::update(float dt)
{
	if(!entered)
		return;

	OM->checkCollisions();

	OM->update(dt);

	EM->processEvents();
}

void gamePlayState::render(void) const
{
	if(!entered)
		return;


	OM->render();

	if(TE)
		TE->Render();

}