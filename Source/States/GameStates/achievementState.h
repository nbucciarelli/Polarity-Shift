//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"achievementState.h"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the achievement state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "menuState.h"
class CSGD_FModManager;

class achievementState : public menuState
{
protected:
	enum menuOptions { BACK, TOTAL };

	int foregroundID;
	float m_fTime, m_fXPer, m_fXLerp, m_fSoundPer, m_fSoundLerp;
	CSGD_FModManager* m_pFMOD;
	bool m_bMainMenu;

	void menuHandler();

	//int m_nParticleImageID;

	achievementState(void);
	achievementState(const achievementState&);
	achievementState& operator=(const achievementState&);
	virtual ~achievementState(void);
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	September 19th, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static achievementState* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	September 19th, 2008
	//	Purpose:		enters the achievement state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	September 19th, 2008
	//	Purpose:		exits the achievement state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"udpate"
	//	Last Modified:	September 19th, 2008
	//	Purpose:		updates the achievement state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	September 19th, 2008
	//	Purpose:		renders the achievement state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;
};