//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CKeyState.h"
//	Author:			Nick Bucciarelli (NB)
//	Purpose:		Handles the key bindings
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "menuState.h"

class CKeyState : public menuState
{
protected:
	enum menuOptions { JUMP, MOVELEFT, MOVERIGHT, BACK, TOTAL };

	int foregroundID;
	float m_fTime, m_fXPer, m_fXLerp;

	char* m_szJump;
	char* m_szMoveLeft;
	char* m_szMoveRight;
	

	void menuHandler();

	CKeyState(void);
	CKeyState(const CKeyState&);
	CKeyState& operator=(const CKeyState&);
	virtual ~CKeyState(void);
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static CKeyState* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		enters the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		exits the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"udpate"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		updates the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		renders the main menu state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void render(void) const;


	//////////////////////////////////////////////////////////////////////////
	//In the options DO THIS NICK!! ZOMG!!! 
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//MAKE AN ACCESSOR AND A SET!!!!;
	//////////////////////////////////////////////////////////////////////////
	//theInput->GetBufferedDIKCodeEx() = game::GetInstance()->GetKeys().m_nJump;
};