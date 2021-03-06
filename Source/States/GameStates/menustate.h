//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"menustate.h"
//	Author:			Scott Smallback (SS) / Nick Bucciarelli (NB) / Jared Hamby (JH)
//	Purpose:		handles the menus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	float m_fTimerNotClicked;	

	bool m_bIsMoving, m_bIsExiting, m_bIsExited;

	int menuPos, cursorID, menuLast, xPos, yPos;

	char** menuItemString;

	unsigned int textColor, highlightColor;

	virtual void menuHandler() = 0;

	menuState(int x = 300, int y = 250,
		unsigned int color = 0xffffffff, unsigned int highlight = 0xffa4a4ff);
	virtual ~menuState(void);
public:
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Enters the menu states
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Exits the menu states
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"input"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Input for the menu states
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool input(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"update"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Updates the menu states
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	August 25, 2008
	//	Purpose:		renders the menus
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Accessors"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Gets the values
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetEmitterPosX() const { return m_nEmitterPosX; }	
	int GetEmitterPosY() const { return m_nEmitterPosY; }
	float GetUpdateTimer() const { return m_fUpdateTimer; }
	int GetYPos() const { return yPos; }
	int GetXPos() const { return xPos; }
	int GetMenuPos() const { return menuPos; }	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Mutators"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Sets the values
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetEmitterPosX(int val) { m_nEmitterPosX = val; }
	void SetEmitterPosY(int val) { m_nEmitterPosY = val; }
	void SetUpdateTimer(float val) { m_fUpdateTimer = val; }
	void SetXPos(int val) { xPos = val; }
	void SetYPos(int val) { yPos = val; }
	void SetMenuPos(int val) { menuPos = val; }
	
	
};
