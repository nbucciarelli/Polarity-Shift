////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "CTallySheetState.cpp"
// Author: Nick Bucciarelli (NB)
// Purpose: This file is the transition between levels
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTallySheet.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\CSGD_FModManager.h"
#include "..\..\game.h"
#include "..\GameStates\levelChooseState.h"

CTallySheetState::CTallySheetState()
{
	somethingtime = 0;
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
	m_nLevelNum = nLevelNum;
	game* theGame = game::GetInstance();
	if (m_fScoreGiven > theGame->GetHighScore(m_nLevelNum))
		theGame->SetHighScore(m_nLevelNum, m_fScoreGiven);
}

void CTallySheetState::enter(void)
{
	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound1()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound1());

	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound2()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound2());
	menuState::enter();
}

void CTallySheetState::exit(void)
{
	somethingtime = 0;
	m_fTempTime = 0;
	m_fTempScore = 0;
	menuState::exit();
}

void CTallySheetState::update(float dt)
{
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
		{
			somethingtime = 0;
			return;
		}

		somethingtime = 0;
	}
}

void CTallySheetState::render(void) const
{
	
	char levelbuffer[32];
	sprintf_s(levelbuffer, "Level %d", m_nLevelNum);

	char buffer1[128];
	sprintf_s(buffer1,"Score : %.2f", m_fTempScore);

	char buffer2[128];
	sprintf_s(buffer2,"Time : %.2f", m_fTempTime);

	theFont->drawText(levelbuffer, 100,100);
	theFont->drawText(buffer1, 100,200);
	theFont->drawText(buffer2, 100,300);
	if(m_fTimeTaken <= game::GetInstance()->GetAchievementNumbers(m_nLevelNum - 1))
	{
		char achievementbuffer[64];
		sprintf_s(achievementbuffer, "Achievement for level %d unlocked!", m_nLevelNum);
		theFont->drawText(achievementbuffer, 100, 400);
	}
}

void CTallySheetState::menuHandler()
{
	switch(menuPos)
	{
	case EXIT:
		//EM->sendGlobalEvent(GE_STATE_POP);
		levelChooseState::getInstance()->SetPlayLevel(levelChooseState::getInstance()->GetPlayLevel()+1);
		if(levelChooseState::getInstance()->GetPlayLevel() < 11)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		else
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_WINSTATE));
		break;
	}
}