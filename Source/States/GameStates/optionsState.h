//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"optionsState.h"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the option menu
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "menuState.h"
class CSGD_FModManager;

class optionsState : public menuState
{
protected:
	enum menuOptions { SFX, MUSIC, KEYBINDINGS, SAVE, LOAD, BACK, TOTAL };

	int foregroundID;
	float m_fTime, m_fXPer, m_fXLerp, m_fSoundPer, m_fSoundLerp;
	bool m_bMainMenu, m_bKeyBind;
	CSGD_FModManager* m_pFMOD;

	void menuHandler();

	//int m_nParticleImageID;

	optionsState(void);
	optionsState(const optionsState&);
	optionsState& operator=(const optionsState&);
	virtual ~optionsState(void);
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	August 25th, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static optionsState* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	August 25, 2008
	//	Purpose:		enters the options state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	August 25, 2008
	//	Purpose:		exits the options state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"udpate"
	//	Last Modified:	August 25, 2008
	//	Purpose:		updates the options state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"input"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Input for the options states
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool input(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	August 25, 2008
	//	Purpose:		renders the options state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;





	//////////////////////////////////////////////////////////////////////////
	//In the options DO THIS NICK!! ZOMG!!! 
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//MAKE AN ACCESSOR AND A SET!!!!;
	//////////////////////////////////////////////////////////////////////////
	//theInput->GetBufferedDIKCodeEx() = game::GetInstance()->GetKeys().m_nJump;
};