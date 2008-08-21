#pragma once

//Forward declarations of classes used within the game
class display;
class renderer;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class gameState;
typedef unsigned long DWORD;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
class eventManager;

#include <vector>
using std::vector;

//Game class: Singleton for a game's operations.  Should contain everything.
class game
{
protected:
	volatile bool isRunning;
	bool isWindowed;

	//Singleton Protection
	game(void);
	game(const game&);
	game& operator=(const game&);
	virtual ~game(void);

	HANDLE updateThread;

	//Component pointers.  Note none care if it's DX or OGL or whatever.
	display* theDisplay;
	renderer* theRenderer;
	inputDevice* theInput;
	eventManager* EM;

	//Time tracking
	DWORD timeStamp;
	DWORD currentTime;
	float dt, gameTime;

	//stateStack:  All states currently in operation
	vector<gameState*> stateStack;
	//Current updating state
	gameState*	currentState;
	
	static unsigned __stdcall updateLoop(void*);

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
};
