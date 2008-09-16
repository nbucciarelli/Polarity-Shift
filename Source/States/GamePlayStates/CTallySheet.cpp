////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "CTallySheetState.cpp"
// Author: Nick Bucciarelli (NB)
// Purpose: This file is the transition between levels
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTallySheet.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Helpers\bitFont.h"

CTallySheetState::CTallySheetState()
{
	m_fTimeTaken = 0.0f;
	m_fScoreGiven = 0.0f;
	m_fTempScore = 0.0f;
	m_fTempTime = 0.0f;
	m_bIsDone = false;
	menuItemString = new char*[TOTAL];

	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

CTallySheetState::~CTallySheetState()
{

}

void CTallySheetState::Initialize(int nLevelNum, float fScoreGiven, float fTimeTaken)
{
	m_fScoreGiven = fScoreGiven;
	m_fTimeTaken = fTimeTaken;
}

void CTallySheetState::enter(void)
{
	menuState::enter();
}

void CTallySheetState::exit(void)
{
	menuState::exit();
}

void CTallySheetState::update(float dt)
{
	static float somethingtime = 0;
	somethingtime += dt;

	if (somethingtime >= .01f)
	{
		if (m_fTempScore >= m_fScoreGiven)
			m_fTempScore = m_fScoreGiven;
		else
			m_fTempScore += 2.12f;

		if (m_fTempTime >= m_fTimeTaken)
			m_fTempTime = m_fTimeTaken;
		else
			m_fTempTime += .42f;

		if (m_fTempTime == m_fTimeTaken && m_fTempScore == m_fScoreGiven)
			return;

		somethingtime = 0;
	}
}

void CTallySheetState::render(void) const
{
	

	char buffer1[128];
	sprintf_s(buffer1,"Score : %.2f", m_fTempScore);

	char buffer2[128];
	sprintf_s(buffer2,"Time : %.2f", m_fTempTime);

	theFont->drawText(buffer1, 100,200);
	theFont->drawText(buffer2, 100,300);

}

void CTallySheetState::menuHandler()
{
	switch(menuPos)
	{
	case EXIT:
		//EM->sendGlobalEvent(GE_STATE_POP);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_LEVELSELECT));
		break;
	}
}