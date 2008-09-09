//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"levelChooseState.h"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the level choosing menu
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "menuState.h"

class levelChooseState : public menuState
{
protected:
	enum menuOptions { LEVEL1, LEVEL2, LEVEL3, BACK, TOTAL };

	int foregroundID;
	float m_fTime, m_fXPer, m_fXLerp;
	bool m_bPlayLevel1, m_bPlayLevel2, m_bPlayLevel3;

	void menuHandler();

	//int m_nParticleImageID;

	levelChooseState(void);
	levelChooseState(const levelChooseState&);
	levelChooseState& operator=(const levelChooseState&);
	virtual ~levelChooseState(void);
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static levelChooseState* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"enter"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		enters the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void enter(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"exit"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		exits the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"udpate"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		updates the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"render"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		renders the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Accessors"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		accesses certain variables in the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool GetPlayLevel1() const { return m_bPlayLevel1; }
	bool GetPlayLevel2() const { return m_bPlayLevel2; }
	bool GetPlayLevel3() const { return m_bPlayLevel3; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Mutators"
	//	Last Modified:	September 9th, 2008
	//	Purpose:		modifies certain variables in the level choose state
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetPlayLevel1(bool val) { m_bPlayLevel1 = val; }
	void SetPlayLevel2(bool val) { m_bPlayLevel2 = val; }
	void SetPlayLevel3(bool val) { m_bPlayLevel3 = val; }
};