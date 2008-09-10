//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"levelChooseState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the options state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "levelChooseState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"
//#include "..\..\Engines\CParticleEffectManager.h"

levelChooseState::levelChooseState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));

	menuItemString = new char*[TOTAL];

	menuItemString[LEVEL1] = "Level 1";
	menuItemString[LEVEL2] = "Level 2";
	menuItemString[LEVEL3] = "Level 3";
	menuItemString[BACK] = "Back";
	menuLast = BACK;
}

levelChooseState::~levelChooseState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

levelChooseState* levelChooseState::getInstance()
{
	static levelChooseState Elgoog;
	return &Elgoog;
}

void levelChooseState::enter(void)
{
	m_bPlayLevel1 = false;
	m_bPlayLevel2 = false;
	m_bPlayLevel3 = false;
	//m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_Test4.prt");
	//CParticleEffectManager::GetInstance()->Play(m_nParticleImageID, true);
	m_fXPer = 0;
	m_fXLerp = 1024;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();
}

void levelChooseState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	menuState::exit();
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
}
void levelChooseState::update(float dt)
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
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
}

void levelChooseState::menuHandler()
{
	switch(menuPos)
	{
	case LEVEL1:
		SetPlayLevel1(true);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case LEVEL2:
		SetPlayLevel2(true);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case LEVEL3:
		SetPlayLevel3(true);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case BACK:
		m_bIsExiting = true;
		break;
	}
}

void levelChooseState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Level Select", (int)(233 + m_fXLerp), 35, textColor, 1.25f);

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 100, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 100, highlightColor);

	//Draw meun cursor at the selected item
	//viewManager::getInstance()->drawTexture(cursorID,
	//	&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	//menuState::render();
	//CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetEmitterPosX(), menuState::GetEmitterPosY()); 
}