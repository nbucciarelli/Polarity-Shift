#include <windows.h>
#include "game.h"
#include "display.h"
#include "Wrappers\dxRenderer.h"
#include "Wrappers\CSGD_DirectInput.h"
#include "Wrappers\viewManager.h"
#include "Helpers\bitFont.h"
#include "EventSystem\eventManager.h"
#include "EventSystem/globalHandler.h"
#include "EventSystem/globalEvents.h"

#include "States\GameStates\mainMenuState.h"
#include "Objects\objFactory.h"
#include "Objects\movingObj.h"

#include <process.h>

#define RENDERER dxRenderer

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

	theRenderer = RENDERER::getInstance();
	theRenderer->Init(theDisplay->getHWnd());
	
	viewManager::getInstance()->initialize(theRenderer);
	bitFont::getInstance()->initialize(viewManager::getInstance());
	
	theInput = inputDevice::GetInstance();
	theInput->InitDirectInput(theDisplay->getHWnd(), hInstance, DI_KEYBOARD | DI_MOUSE);

	EM = eventManager::getInstance();
	globalHandler::getInstance()->initialize();
	
	isWindowed = true;
	theRenderer->changeResolution(DEFAULT_WNDHEIGHT, DEFAULT_WNDWIDTH, isWindowed);

	globalHandler::getInstance()->HandleEvent(&gameEvent(GE_GAME_START));

	timeStamp = GetTickCount();
	srand(timeStamp);
}

void game::Shutdown()
{
	changeState(NULL);
	
	WaitForSingleObject(renderThread, INFINITE);

	objFactory::getInstance()->unregisterAll();

	globalHandler::getInstance()->shutdown();
	
	theInput->ShutdownDirectInput();

	bitFont::getInstance()->shutdown();
	//viewManager::getInstance()->shutdown();

	theRenderer->Shutdown();

	theDisplay->ShutdownWindow();

	viewManager::getInstance()->shutdown();
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
	renderThread = (HANDLE)_beginthreadex(0, stackSize,
		renderLoop, 0, 0, 0);


	while(isRunning)
	{
		theDisplay->HandleWindowMsgs();

		currentTime = GetTickCount();

		dt = (currentTime - timeStamp) / 1000.0f;
		timeStamp = currentTime;
		gameTime += dt;

		theInput->ReadDevices();

		if(getInput() &&currentState)
			currentState->input(dt);

		if(currentState)
			currentState->update(dt);
		else
			isRunning = false;

		EM->processGlobalEvents();
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

#pragma region render loop
unsigned game::renderLoop(void* unused)
{
	game* g = GetInstance();
	while ( g->isRunning )
	{
		if(g->currentState)
		{
			g->theRenderer->BeginScene();
			g->theRenderer->BeginSprites();

			for(unsigned c = 0; c < g->stateStack.size(); c++)
				g->stateStack[c]->render();

			g->theRenderer->EndSprites();
			g->theRenderer->EndScene();
		}
	}
	return 0;
}
#pragma endregion