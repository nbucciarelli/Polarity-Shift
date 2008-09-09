//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CKeyState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the Keys state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CKeyState.h"
#include <windows.h>
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Wrappers\viewManager.h"
#include "../../Helpers/SGD_Math.h"
#include "..\..\game.h"
#include "..\..\Helpers\bitFont.h"

CKeyState::CKeyState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));

	menuItemString = new char*[TOTAL];

	menuItemString[JUMP] = "Jump: 0";
	menuItemString[MOVELEFT] = "Move Left: ";
	menuItemString[MOVERIGHT] = "Move Right: ";
	menuItemString[BACK] = "Back";
	menuLast = BACK;

	//strcpy_s(m_szJump, sizeof((char*)game::GetInstance()->GetKeys().m_nJump),(char*)game::GetInstance()->GetKeys().m_nJump) ;
	//m_szMoveLeft = (char*)game::GetInstance()->GetKeys().m_nRunLeft;
}

CKeyState::~CKeyState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

CKeyState* CKeyState::getInstance()
{
	static CKeyState Elgoog;
	return &Elgoog;
}

void CKeyState::enter(void)
{

	m_fXPer = 0;
	m_fXLerp = 0;
	m_bIsMoving = true;
	menuState::enter();
}

void CKeyState::exit(void)
{
	m_bIsMoving = true;
	menuState::exit();
	
}
void CKeyState::update(float dt)
{
	if(!entered)
		return;

	m_fTime += dt; 
	if(m_bIsMoving == true)
	{
		if (dt >= .016f) 
		{ 
			m_fXPer += .02f; 
			m_fXLerp = Lerp(1024, 0, m_fXPer); 
			if(m_fXPer >= 1)
			{
				m_fXPer = 1;
				m_bIsMoving = false;
			}
		}
	}
}

void CKeyState::menuHandler()
{
	switch(menuPos)
	{
	case JUMP:
	case MOVELEFT:
		break;
	case MOVERIGHT:
		break;
	case BACK:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_OPTIONS));
		break;
	}
}

void CKeyState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], 20 + m_fXLerp + xPos, yPos + c * 50, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], 20 + m_fXLerp + xPos, yPos + c * 50, highlightColor);

	//Draw meun cursor at the selected item
	viewManager::getInstance()->drawTexture(cursorID,
		&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

}