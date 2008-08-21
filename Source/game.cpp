#include <windows.h>
#include "game.h"
#include "display.h"
#include "dxRenderer.h"
#include "CSGD_DirectInput.h"
#include "viewManager.h"
#include "bitFont.h"
#include "eventManager.h"

#include "mainMenuState.h"
#include "objFactory.h"
#include "movingObj.h"

#include <process.h>

//#define STARTSTATE mainMenuState::getInstance()
#define STARTSTATE NULL
#define RENDERER dxRenderer::getInstance();

#pragma region constructor/destructor/singleton
game::game(void) : isRunning(true), isWindowed(true), currentState(NULL), gameTime(0) {}

game::~game(void) {}

game * game::GetInstance()
{
	static game theInstance;
	return &theInstance;
}
#pragma endregion

#pragma region initialize/shutdown
void game::Initialize(HINSTANCE hInstance)
{
	theDisplay = display::getInstance();
	theDisplay->InitWindow(hInstance);

	if(!theDisplay->getHWnd())
		return;

	theRenderer = RENDERER;
	theRenderer->Init(theDisplay->getHWnd());
	
	viewManager::getInstance()->initialize(theRenderer);
	bitFont::getInstance()->initialize(viewManager::getInstance());
	
	theInput = inputDevice::GetInstance();
	theInput->InitDirectInput(theDisplay->getHWnd(), hInstance, DI_KEYBOARD | DI_MOUSE);

	EM = eventManager::getInstance();
	
	isWindowed = true;
	theRenderer->changeResolution(DEFAULT_WNDHEIGHT, DEFAULT_WNDWIDTH, isWindowed);

	changeState(STARTSTATE);

	timeStamp = GetTickCount();
	srand(timeStamp);
}

void game::Shutdown()
{
	changeState(NULL);
	
	WaitForSingleObject(updateThread, INFINITE);

	objFactory::getInstance()->unregisterAll();

	theInput->ShutdownDirectInput();

	bitFont::getInstance()->shutdown();
	viewManager::getInstance()->shutdown();

	theRenderer->Shutdown();

	theDisplay->ShutdownWindow();
}
#pragma endregion

#pragma region state machine
void game::changeState(gameState* newState)
{
	clearStates();

	currentState = newState;

	if(currentState)
	{
		stateStack.push_back(currentState);
		currentState->enter();
	}
}
void game::pushState(gameState* state)
{
	if(!state) //Never put NULL in the state stack
		return;

	stateStack.push_back(state);
	currentState = state;
	currentState->enter();
}

void game::popState()
{
	if(stateStack.empty())
		return;

	currentState->exit();

	stateStack.pop_back();

	if(stateStack.empty())
		currentState = NULL;
	else
		currentState = stateStack[stateStack.size() - 1];
}

void game::clearStates()
{
	currentState = NULL;

	//Grab the topmost state, tell it it's finished, remove, repeat
	gameState* running = NULL;
	while(!stateStack.empty())
	{
		running = stateStack.back();
		running->exit();
		stateStack.pop_back();
	}
}
#pragma endregion

#pragma region run funcs
void game::Run()
{
	if(!theDisplay->getHWnd())
		return;

	const int stackSize = 65536;
	updateThread = (HANDLE)_beginthreadex(0, stackSize,
		updateLoop, 0, 0, 0);

	while ( isRunning )
	{
		if(currentState)
		{
			theRenderer->BeginScene();
			theRenderer->BeginSprites();

			for(unsigned c = 0; c < stateStack.size(); c++)
				stateStack[c]->render();

			theRenderer->EndSprites();
			theRenderer->EndScene();
		}
	}
}

bool game::getInput()
{
	if(theInput->KeyPressed(DIK_RETURN))
	{
		if(theInput->KeyDown(DIK_LALT) || theInput->KeyDown(DIK_RALT))
		{
			isWindowed = !isWindowed;

			theRenderer->changeResolution(DEFAULT_WNDHEIGHT, DEFAULT_WNDWIDTH, isWindowed);

			return false;
		}
	}
	return true;
}
#pragma endregion

#pragma region update loop
unsigned game::updateLoop(void* unused)
{
	game* g = GetInstance();
	while(g->isRunning)
	{
		g->theDisplay->HandleWindowMsgs();

		g->currentTime = GetTickCount();

		g->dt = (g->currentTime - g->timeStamp) / 1000.0f;
		g->timeStamp = g->currentTime;
		g->gameTime += g->dt;

		g->theInput->ReadDevices();

		if(g->getInput() &&g->currentState)
			g->currentState->input(g->dt);

		if(g->currentState)
			g->currentState->update(0.016f);
		else
			g->isRunning = false;

		g->EM->processGlobalEvents();
	}
	return 0;
}
#pragma endregion