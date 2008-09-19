//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"achievementState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the achievement state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "achievementState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\CSGD_FModManager.h"

achievementState::achievementState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");

	menuItemString = new char*[TOTAL];

	menuItemString[BACK] = "Back";
	menuLast = BACK;
}

achievementState::~achievementState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

achievementState* achievementState::getInstance()
{
	static achievementState Elgoog;
	return &Elgoog;
}

void achievementState::enter(void)
{
	m_fSoundLerp = 100;
	m_fXLerp = 1024;
	m_bMainMenu = false;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();
}

void achievementState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	m_fSoundLerp = 100;
	menuState::exit();
}
void achievementState::update(float dt)
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
		if(m_bMainMenu == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
	}
}

void achievementState::menuHandler()
{
	switch(menuPos)
	{
	case BACK:
		m_bIsExiting = true;
		m_bMainMenu = true;
		break;
	}
}

void achievementState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Achievements", (int)(218 + m_fXLerp), 35, textColor, 1.25f);

	//Draw menu items
	theFont->drawText("Complete level 1 in 10 secs", (int)(105 + m_fXLerp + xPos), 300, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);
	theFont->drawText("Complete level 2 in 15 secs", (int)(105 + m_fXLerp + xPos), 340, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);
	theFont->drawText("Complete level 3 in 10 secs", (int)(105 + m_fXLerp + xPos), 380, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);
	theFont->drawText("Complete level 4 in 15 secs", (int)(105 + m_fXLerp + xPos), 420, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);
	theFont->drawText("Complete level 5 in 15 secs", (int)(105 + m_fXLerp + xPos), 460, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);
	theFont->drawText("Complete level 6 in 10 secs", (int)(105 + m_fXLerp + xPos), 500, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128), .55f);

	theFont->drawText(menuItemString[BACK], (int)(20 + m_fXLerp + xPos), 675, highlightColor);
}