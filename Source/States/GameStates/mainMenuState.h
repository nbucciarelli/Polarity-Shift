//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"mainManuState.h"
//	Author:			Scott Smallback (SS) / Nick Bucciarelli (NB) / Jared Hamby (JH)
//	Purpose:		handles the main menus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "menuState.h"

class mainMenuState : public menuState
{
protected:
	enum menuOptions { PLAY, OPTIONS, HOWTO, SCORES, CREDITS,
		
		EXIT, TOTAL
	};

	int foregroundID;
	float m_fTime, m_fXPer, m_fXLerp;

	void menuHandler();

	int m_nParticleImageID;

	mainMenuState(void);
	mainMenuState(const mainMenuState&);
	mainMenuState& operator=(const mainMenuState&);
	virtual ~mainMenuState(void);
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	August 25th, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static mainMenuState* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	August 25, 2008
	//	Purpose:		enters the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	August 25, 2008
	//	Purpose:		exits the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"udpate"
	//	Last Modified:	August 25, 2008
	//	Purpose:		updates the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	August 25, 2008
	//	Purpose:		renders the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;
};