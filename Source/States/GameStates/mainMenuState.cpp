//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"mainManuState.cpp"
//	Author:			Scott Smallback (SS) / Nick Bucciarelli (NB) / Jared Hamby (JH)
//	Purpose:		handles the main menus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mainMenuState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\CSGD_FModManager.h"
//#include "..\..\Wrappers\CVideoMaster.h"

#include "..\..\Engines\CParticleEffectManager.h"

mainMenuState::mainMenuState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");
	menuItemString = new char*[TOTAL];

	menuItemString[PLAY] = "Play";
	menuItemString[OPTIONS] = "Options";
	menuItemString[HOWTO] = "How To Play";
	menuItemString[ACHIEVEMENTS] = "Achievements";
	menuItemString[SCORES] = "High Scores";
	menuItemString[CREDITS] = "Credits";
	menuItemString[EXIT] = "Exit";
	menuLast = EXIT;
}

mainMenuState::~mainMenuState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

mainMenuState* mainMenuState::getInstance()
{
	static mainMenuState Elgoog;
	return &Elgoog;
}

void mainMenuState::enter(void)
{
	m_fXPer = 0;
	m_fXLerp = 1024;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	m_bLevelSelect = false;
	m_bOptions = false;
	m_bHighScores = false;
	m_bAchievements	 = false;
	m_bHowTo = false;
	m_bCredits = false;

	m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_SmokeBottomRight.prt");
	CParticleEffectManager::GetInstance()->Play(m_nParticleImageID, true);


	menuState::enter();
}

void mainMenuState::exit(void)
{
	m_fXLerp = 1024;
	menuState::exit();
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
	CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);

}
void mainMenuState::update(float dt)
{
	if(!entered)
		return;



	m_fTime += dt; 
	if(m_bIsMoving == true)
	{
		if (dt >= .016f) 
		{ 
			m_fXPer += .1f; 
			m_fXLerp = Lerp(1024, 0, m_fXPer); 
			m_fSoundPer -= .2f;
			m_fSoundLerp = Lerp(100, 0, m_fXPer);
			m_fSoundLerp *= -1;
			// SET PAN FROM RIGHT TO CENTER WITH SOUND LERP
			if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetSZSCHHHSound()))
				CSGD_FModManager::GetInstance()->SetPan(game::GetInstance()->GetSZSCHHHSound(), m_fSoundLerp);
			// PLAY SOUND HERE
			CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			if(m_fXPer >= 1)
			{
				m_fXPer = 1;
				// STOP SOUND HERE
				CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetSZSCHHHSound());
				m_bIsMoving = false;
			}
		}
	}
	else if(m_bIsExiting == true)
	{
		if (dt >= .016f) 
		{ 
			m_fXPer -= .1f; 
			m_fXLerp = Lerp(1024, 0, m_fXPer);
			m_fSoundPer -= .2f;
			m_fSoundLerp = Lerp(0, 100, m_fSoundPer);
			m_fSoundLerp *= -1;
			// SET PAN FROM CENTER TO RIGHT WITH SOUND LERP
			CSGD_FModManager::GetInstance()->SetPan(game::GetInstance()->GetSZSCHHHSound(), m_fSoundLerp);
			// PLAY SOUND HERE
			if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetSZSCHHHSound()))
				CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			if(m_fXPer <= 0)
			{
				m_fXPer = 0;
				// STOP SOUND HERE
				CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetSZSCHHHSound());
				m_bIsExiting = false;
				m_bIsExited = true;
			}
		}
	}

	if(m_bIsExited == true)
	{
		if(m_bOptions == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_OPTIONS));
		else if(m_bLevelSelect == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_LEVELSELECT));
		else if(m_bHighScores == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_HIGHSCORES));
		else if(m_bCredits == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_CREDITS));
		else if(m_bAchievements == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_ACHIEVEMENTS));
	}

	CParticleEffectManager::GetInstance()->Update(dt);


}

void mainMenuState::menuHandler()
{
	switch(menuPos)
	{
	case PLAY:
		//game::GetInstance()->changeState(playState::getInstance());
		m_bIsExiting = true;
		m_bLevelSelect = true;
		break;
	case OPTIONS:
		m_bIsExiting = true;
		m_bOptions = true;
		break;
	case HOWTO:
		m_bIsExiting = true;
		m_bHowTo = true;
		break;
	case ACHIEVEMENTS:
		m_bIsExiting = true;
		m_bAchievements = true;
		break;
	case SCORES:
		m_bIsExiting = true;
		m_bHighScores = true;
		break;
	case CREDITS:
		m_bIsExiting = true;
		m_bCredits = true;
		break;
	default:		
		EM->sendGlobalEvent(GE_STATE_CLEARALL);
	}
}

void mainMenuState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Polarity Shift", (int)(193 + m_fXLerp), 35, textColor, 1.25f);

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], (int)(m_fXLerp + 320), yPos + c * 90, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], (int)(m_fXLerp + 320), yPos + c * 90, highlightColor);

	//Draw meun cursor at the selected item
	//viewManager::getInstance()->drawTexture(cursorID,
	//	&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	//CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetXPos(), menuState::GetYPos()+ 10 + menuState::GetMenuPos() * 50); 
	CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, 1024, 600); 
}