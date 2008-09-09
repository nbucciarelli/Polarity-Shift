////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �gamePlayState.cpp�
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the game play state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\eventIDs.h"
#include "../../EventSystem/gameEvent.h"
#include "../../EventSystem/playHandler.h"
#include "../../Objects/objManager.h"
#include "..\..\Engines\CTileEngine.h"
#include "..\..\Engines\CAIEngine.h"
#include "..\..\game.h"

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

	for(int c = BEGIN_PLAY_EVENTS; c < END_PLAY_EVENTS; c++)
		EM->registerClient(c, this);

	EM->sendEvent(EVENT_GAMELOADING);

	AIE = new CAIEngine();
	TE = new CTileEngine();
	TE->LoadMap("Resource/PS_TestLevel.bmf");
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

	if (theInput->KeyPressed(DIK_ESCAPE) || theInput->KeyPressed(DIK_F10))
		EM->sendEvent(EVENT_GAMEPAUSE);

	if(entered)
	{
		if(theInput->KeyDown(game::GetInstance()->GetKeys().m_nRunLeft))
		EM->sendEvent(EVENT_PLAYERGOLEFT);
	else if(theInput->KeyDown(game::GetInstance()->GetKeys().m_nRunRight))
		EM->sendEvent(EVENT_PLAYERGORIGHT);
	else if(theInput->KeyReleased(DIK_A) || theInput->KeyReleased(DIK_D))
		EM->sendEvent(EVENT_PLAYERSTOP);

	if(theInput->KeyPressed(game::GetInstance()->GetKeys().m_nJump))
		EM->sendEvent(EVENT_PLAYERJUMP);
	}

	return true;
}

void gamePlayState::update(float dt)
{
	if(entered)
	{
		AIE->update();

		OM->checkCollisions();

		OM->update(dt);
	}

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

void gamePlayState::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case EVENT_LEVELLOADED:
		entered = true;
		break;
	case EVENT_LEVELFINISHED:
		entered = false;
		break;
	}
}