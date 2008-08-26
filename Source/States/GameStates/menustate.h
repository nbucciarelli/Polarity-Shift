#pragma once
#include "..\gameState.h"

class objManager;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class bitFont;
class eventManager;
class CXBOXController;

class menuState : public gameState
{
protected:
	objManager * objM;
	inputDevice* theInput;
	bitFont* theFont;
	eventManager* EM;
	CXBOXController* Player1;
	bool m_bIsBuffered;
	int m_nEmitterPosX;
	int m_nEmitterPosY;
	float m_fUpdateTimer;
	
	
	
	

	int menuPos, cursorID, menuLast, xPos, yPos;
	char** menuItemString;

	unsigned int textColor, highlightColor;

	virtual void menuHandler() = 0;

	menuState(int x = 100, int y = 100,
		unsigned int color = 0xff66bb66, unsigned int highlight = 0xff88dd88);
	virtual ~menuState(void) {}
public:

	virtual void enter(void);
	virtual void exit(void);

	virtual bool input(float dt);
	virtual void update(float dt);
	virtual void render(void) const;

	int GetEmitterPosX() const { return m_nEmitterPosX; }
	void SetEmitterPosX(int val) { m_nEmitterPosX = val; }
	int GetEmitterPosY() const { return m_nEmitterPosY; }
	void SetEmitterPosY(int val) { m_nEmitterPosY = val; }
	float GetUpdateTimer() const { return m_fUpdateTimer; }
	void SetUpdateTimer(float val) { m_fUpdateTimer = val; }
};
