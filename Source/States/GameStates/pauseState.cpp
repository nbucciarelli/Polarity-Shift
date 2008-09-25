////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “pauseState.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the pause state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "pauseState.h"
#include "mainMenuState.h"
#include "..\..\Helpers\bitFont.h"
#include "..\..\game.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\Wrappers\viewManager.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\CSGD_FModManager.h"

#pragma region constructor/destructor, singleton
pauseState::pauseState(void) : menuState(700, 400)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Pause.png");

	menuItemString = new char*[TOTAL];
	menuLast = BACK;

	menuItemString[RESUME] = "Resume";
	menuItemString[BACK] = "Exit";
}

pauseState::~pauseState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

pauseState* pauseState::getInstance()
{
	static pauseState elevator;
	return &elevator;
}
#pragma endregion

#pragma region enter/exit
void pauseState::enter(void)
{
	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound1()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound1());

	if(CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound2()))
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound2());

	m_fSoundLerp = 100;
	m_fXLerp = 1024;
	m_bBack = false;
	m_bResume = false;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();

	EM = eventManager::getInstance();
}
void pauseState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	m_fSoundLerp = 100;
	menuState::exit();
}
#pragma endregion

#pragma region frame cycle
bool pauseState::input(float dt)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		m_bResume = true;
		m_bIsExiting = true;
	}

	return menuState::input(dt);
}

void pauseState::update(float dt)
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
		if(m_bBack == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_LEVELSELECT));
		else if(m_bResume == true)
			EM->sendGlobalEvent(GE_STATE_POP);
	}
}

void pauseState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Paused", (int)(383 + m_fXLerp), 128, textColor, 1);

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + 300), yPos + c * 100, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + 300), yPos + c * 100, highlightColor);
}
#pragma endregion

void pauseState::menuHandler()
{
	switch(menuPos)
	{
	case RESUME:
		m_bResume = true;
		m_bIsExiting = true;
		break;
	case BACK:
		m_bBack = true;
		m_bIsExiting = true;
		break;
	}
}