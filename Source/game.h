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

#define NUMLEVELS 10

//Game class: Singleton for a game's operations.  Should contain everything.
class game
{
protected:
	volatile bool isRunning;
	volatile bool isWindowed;
	volatile bool renderLock;

	int m_nVideoID[2];
	
	
	bool IsTutorialDone;
	bool m_bLevelsComplete[NUMLEVELS];
	bool m_bAchievementUnlock[NUMLEVELS];
	float m_fAchievementTimes[NUMLEVELS];
	int m_nMusicLevel, m_nSFXLevel, m_nFreqLevel;

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
	int m_SZSCHHHSound, m_nGunSound[3], m_nPowerUpSound, m_nBossMusic, m_nLevelMusic, m_nDeathSound;

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
	
	int GetSZSCHHHSound() const { return m_SZSCHHHSound; }

	bool* GetLevelComplete() { return m_bLevelsComplete;}
	void SetLevelComplete(int level) { m_bLevelsComplete[level + 1] = true; }
	bool* GetAchievementUnlock() { return m_bAchievementUnlock; }
	void SetAchievementUnlock(int level) { m_bAchievementUnlock[level] = true; }
	float GetAchievementNumbers(int level) { return m_fAchievementTimes[level]; }
	int GetMusicLevel() const { return m_nMusicLevel; }
	void SetMusicLevel(int val) { m_nMusicLevel = val; }
	int GetSFXLevel() const { return m_nSFXLevel; }
	void SetSFXLevel(int val) { m_nSFXLevel = val; }
	int GetFreqLevel() const { return m_nFreqLevel; }
	void SetFreqLevel(int val) { m_nFreqLevel = val; }

	bool GetTutorialDone() {return IsTutorialDone;}
	void SetTutorialDone(bool val) {IsTutorialDone = val;}
	
	int GetVideoID2() const { return m_nVideoID[1]; }
	//m_nGunSound, m_nPowerUpSound, m_nBossMusic, m_nLevelMusic, m_nDeathSound;
	int GetGunSound1() { return m_nGunSound[0]; }
	int GetGunSound2() { return m_nGunSound[1]; }
	int GetGunSound3() { return m_nGunSound[2]; }
	int GetPowerUpSound() { return m_nPowerUpSound; }
	int GetBossMusic() { return m_nBossMusic; }
	int GetLevelMusic() { return m_nLevelMusic; }
	int GetDeathSound() { return m_nDeathSound; }
};
