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
#include "..\..\Engines\CParticleEffectManager.h"

mainMenuState::mainMenuState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));
	menuItemString = new char*[TOTAL];

	menuItemString[PLAY] = "Play";
	menuItemString[OPTIONS] = "Options";
	menuItemString[HOWTO] = "How To Play";
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
	m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_CursorParticle.prt");
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
			if(m_fXPer >= 1)
			{
				m_fXPer = 1;
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
			if(m_fXPer <= 0)
			{
				m_fXPer = 0;
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
		break;
	case SCORES:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_HIGHSCORES));
		break;
	case CREDITS:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int (STATE_CREDITS));
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

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 50, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 50, highlightColor);

	//Draw meun cursor at the selected item
	viewManager::getInstance()->drawTexture(cursorID,
		&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetXPos(), menuState::GetYPos()+ 10 + menuState::GetMenuPos() * 50); 
	
}