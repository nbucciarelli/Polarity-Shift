#pragma once

//Forward declarations of classes used within the game
class display;
//class renderer;
class dxRenderer;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class gameState;
typedef unsigned long DWORD;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
typedef void* HANDLE;
class eventManager;
class CSGD_FModManager;

#include "Wrappers/CTimer.h"
#include "Helpers\CKeyBindings.h"
#include "Wrappers\CPixelShader.h"
//class CPixelShader;

#include <vector>
using std::vector;

//Game class: Singleton for a game's operations.  Should contain everything.
class game
{
protected:
	volatile bool isRunning;
	volatile bool isWindowed;
	volatile bool renderLock;

	int m_nVideoID;



	//Singleton Protection
	game(void);
	game(const game&);
	game& operator=(const game&);
	virtual ~game(void);

	HANDLE renderThread;

	
	Keys m_cKeys;
		
	//Component pointers.  Note none care if it's DX or OGL or whatever.
	display* theDisplay;
	dxRenderer* theRenderer;
	inputDevice* theInput;
	eventManager* EM;
	CSGD_FModManager* m_pFMOD;

	CPixelShader			m_pixelShader;
	LPDIRECT3DTEXTURE9		m_pRenderTarget;			//	Where we're going to render the scene to.
	LPDIRECT3DSURFACE9		m_pBackbuffer;				//  temp back buffer pointer.

	CTimer					m_timer;

	//Time tracking
	DWORD timeStamp;
	DWORD currentTime;
	float dt, gameTime;

	//stateStack:  All states currently in operation
	vector<gameState*> stateStack;
	//Current updating state
	gameState*	currentState;
	
	static unsigned __stdcall renderLoop(void*);

	//For any "global" input actions that may be needed.
	//return tells whether or not to skip state input
	bool getInput();

public:
	static game * GetInstance();

	void Initialize(HINSTANCE hInstance);
	void Run();
	void Shutdown();

	//State machine operations
	void changeState(gameState*);
	void pushState(gameState*);
	void popState();
	void clearStates();

	bool getIsRunning() { return isRunning; }
	void setIsRunning(bool set) { isRunning = set; }
	float getGameTime() { return gameTime; }

	Keys GetKeys(){return m_cKeys;}
	void SetJump(unsigned val) { m_cKeys.m_nJump = val; }
	void SetMoveLeft(unsigned val) { m_cKeys.m_nRunLeft = val; }
	void SetMoveRight(unsigned val) { m_cKeys.m_nRunRight = val; }
	
	
};
