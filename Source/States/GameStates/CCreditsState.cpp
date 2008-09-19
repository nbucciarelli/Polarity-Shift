//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CCreditsState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the credits state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CCreditsState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\CSGD_FModManager.h"

CCreditsState::CCreditsState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));

	menuItemString = new char*[TOTAL];

	menuItemString[BACK] = "Back";
	menuLast = BACK;
}

CCreditsState::~CCreditsState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

CCreditsState* CCreditsState::getInstance()
{
	static CCreditsState Elgoog;
	return &Elgoog;
}

void CCreditsState::enter(void)
{
	m_fSoundLerp = 100;
	m_fXLerp = 1024;
	m_bMainMenu = false;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();
}

void CCreditsState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	m_fSoundLerp = 100;
	menuState::exit();
}
void CCreditsState::update(float dt)
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

void CCreditsState::menuHandler()
{
	switch(menuPos)
	{
	case BACK:
		m_bIsExiting = true;
		m_bMainMenu = true;
		break;
	}
}

void CCreditsState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Options", (int)(303 + m_fXLerp), 35, textColor, 1.25f);

	//Draw menu items
	theFont->drawText("Nick Bucciarelli - Project Officer", (int)(100 + m_fXLerp + xPos), 300, textColor, .55f);
	theFont->drawText("Jared Hamby - Interface Officer", (int)(100 + m_fXLerp + xPos), 340, textColor, .55f);
	theFont->drawText("Lee Nyman - Gameplay Officer", (int)(100 + m_fXLerp + xPos), 380, textColor, .55f);
	theFont->drawText("Scott Smallback - Technical Officer", (int)(100 + m_fXLerp + xPos), 420, textColor, .55f);

	theFont->drawText("Dustin Clingman - Executive Producer", (int)(100 + m_fXLerp + xPos), 500, textColor, .55f);
	theFont->drawText("Ronald Powell - Associate Producer", (int)(100 + m_fXLerp + xPos), 540, textColor, .55f);
	theFont->drawText(menuItemString[BACK], (int)(20 + m_fXLerp + xPos), 675, highlightColor);
}