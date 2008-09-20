//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"howToState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the how to state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "howToState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\Wrappers\CSGD_FModManager.h"

#define TEXTSIZE .45f

howToState::howToState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");

	menuItemString = new char*[TOTAL];

	menuItemString[BACK] = "Back";
	menuLast = BACK;
}

howToState::~howToState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

howToState* howToState::getInstance()
{
	static howToState Elgoog;
	return &Elgoog;
}

void howToState::enter(void)
{
	m_fSoundLerp = 100;
	m_fXLerp = 1024;
	m_bMainMenu = false;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();
}

void howToState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	m_fSoundLerp = 100;
	menuState::exit();
}
void howToState::update(float dt)
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

void howToState::menuHandler()
{
	switch(menuPos)
	{
	case BACK:
		m_bIsExiting = true;
		m_bMainMenu = true;
		break;
	}
}

void howToState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("How To Play", (int)(303 + m_fXLerp), 65, textColor, 1);

	//Draw menu items
	theFont->drawText("'A' to go left", (int)(20 + m_fXLerp + xPos), 280, textColor);
	theFont->drawText("'D' to go right", (int)(20 + m_fXLerp + xPos), 320, textColor);
	theFont->drawText("'W' to jump", (int)(20 + m_fXLerp + xPos), 360, textColor);
	theFont->drawText("Left click to push", (int)(20 + m_fXLerp + xPos), 400, textColor);
	theFont->drawText("Right click to pull", (int)(20 + m_fXLerp + xPos), 440, textColor);
	theFont->drawText("Both clicked to grab", (int)(20 + m_fXLerp + xPos), 480, textColor);

	theFont->drawText(menuItemString[BACK], (int)(20 + m_fXLerp + xPos), 675, highlightColor);
}