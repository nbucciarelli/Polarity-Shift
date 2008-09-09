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
#include "..\..\Wrappers\CSGD_DirectInput.h"

CKeyState::CKeyState(void) : state(NULL), character(0)
{
	m_szJump = new char[128];
	m_szMoveLeft = new char[128];
	m_szMoveRight = new char[128];

	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));
	
	char* buffer;
	
	IsPressed = false;

	m_nJump = game::GetInstance()->GetKeys().m_nJump;
	m_nMoveLeft = game::GetInstance()->GetKeys().m_nRunLeft;
	m_nMoveRight = game::GetInstance()->GetKeys().m_nRunRight;

	menuItemString = new char*[TOTAL];

	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nJump);
	sprintf_s(m_szJump, 128, "Jump: %s", buffer);
	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nRunLeft);
	sprintf_s(m_szMoveLeft, 128, "Move Left: %s", buffer);
	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nRunRight);
	sprintf_s(m_szMoveRight, 128, "Move Left: %s", buffer);


	menuItemString[JUMP] = m_szJump;
	menuItemString[MOVELEFT] = m_szMoveLeft;
	menuItemString[MOVERIGHT] = m_szMoveRight;
	menuItemString[BACK] = "Back";
	menuLast = BACK;
	

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
	m_fXLerp = 1024;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	m_bIsHighlighted = false;
	menuState::enter();
}

void CKeyState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	delete[] m_szJump;
	delete[] m_szMoveLeft;
	delete[] m_szMoveRight;
	menuState::exit();
}
bool CKeyState::input(float dt)
{
	if(state == KB_WAITING)
		return true;

	return menuState::input(dt);
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


	switch(state)
	{
	case KB_BEGINCHECK:
		if(theInput->KeyReleased(DIK_RETURN))
		{
			state = KB_WAITING;
		}
		break;
	case KB_WAITING:
		
		if(theInput->KeyPressed(DIK_ESCAPE))
			state = KB_NULL;

		if(character = theInput->GetBufferedDIKCodeEx())
		{
			switch(menuPos)
			{
			case JUMP:
				if (game::GetInstance()->GetKeys().m_nJump != character)
					game::GetInstance()->SetJump(character);
				else
					MessageBox(0, "Insert a different key", "Key Already Used", MB_OK);
				
				break;
			case MOVELEFT:
				if (game::GetInstance()->GetKeys().m_nRunLeft != character)
					game::GetInstance()->SetMoveLeft(character);
				else
					MessageBox(0, "Insert a different key", "Key Already Used",MB_OK);
				
				break;
			case MOVERIGHT:
				if (game::GetInstance()->GetKeys().m_nRunRight != character)
					game::GetInstance()->SetMoveRight(character);
				else
					MessageBox(0, "Insert a different key", "Key Already Used",MB_OK);
				
			break;
			case BACK:
				m_bIsExiting = true;
				break;
			}

			
			//MessageBox(0, ":(", ":O", MB_OK);
			state = KB_NULL;
			highlightColor = oldhighlight;
		}
	default:
		break;
	}

	char* buffer;
	
	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nJump);
	sprintf_s(m_szJump, 128, "Jump: %s", buffer);
	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nRunLeft);
	sprintf_s(m_szMoveLeft, 128, "Move Left: %s", buffer);
	buffer = SetKeyString(game::GetInstance()->GetKeys().m_nRunRight);
	sprintf_s(m_szMoveRight, 128, "Move Left: %s", buffer);


	menuItemString[JUMP] = m_szJump;
	menuItemString[MOVELEFT] = m_szMoveLeft;
	menuItemString[MOVERIGHT] = m_szMoveRight;
	menuItemString[BACK] = "Back";
	menuLast = BACK;

	if(m_bIsExited == true)
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_OPTIONS));


}

void CKeyState::menuHandler()
{
	switch(menuPos)
	{
	case JUMP:
	case MOVELEFT:
	case MOVERIGHT:
		oldhighlight = highlightColor;
		highlightColor = 0xffff0000;
		menuState::render();
		state = KB_BEGINCHECK;
		break;
	case BACK:
		m_bIsExiting = true;
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
			theFont->drawText(menuItemString[c], 20 + (int)m_fXLerp + xPos, yPos + c * 50, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], 20 + (int)m_fXLerp + xPos, yPos + c * 50, highlightColor);

	//Draw meun cursor at the selected item
	viewManager::getInstance()->drawTexture(cursorID,
		&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

}

char* CKeyState::SetKeyString(unsigned int nKey)
{
	char* szString;

#pragma region "Bitchin' huge switch"
	switch(nKey)
	{
	case 0x0F:
		szString = "Tab";
		break;
	case 0x10:
		szString = "Q";
		break;
	case 0x11:
		szString = "W";
		break;
	case 0x12:
		szString = "E";
		break;
	case 0x13:
		szString = "R";
		break;
	case 0x14:
		szString = "T";
		break;
	case 0x15:
		szString = "Y";
		break;
	case 0x16:
		szString = "U";
		break;
	case 0x17:
		szString = "I";
		break;
	case 0x18:
		szString = "O";
		break;
	case 0x19:
		szString = "P";
		break;
	case 0x1C:
		szString = "ENTER";
		break;
	case 0x1E:
		szString = "A";
		break;
	case 0x1F:
		szString = "S";
		break;
	case 0x20:
		szString = "D";
		break;
	case 0x21:
		szString = "F";
		break;
	case 0x22:
		szString = "G";
		break;
	case 0x23:
		szString = "H";
		break;
	case 0x24:
		szString = "J";
		break;
	case 0x25:
		szString = "K";
		break;
	case 0x26:
		szString = "L";
		break;
	case 0x2C:
		szString = "Z";
		break;
	case 0x2D:
		szString = "X";
		break;
	case 0x2E:
		szString = "C";
		break;
	case 0x2F:
		szString = "V";
		break;
	case 0x30:
		szString = "B";
		break;
	case 0x31:
		szString = "N";
		break;
	case 0x32:
		szString = "M";
		break;
	case 0x36:
		szString = "R Shift";
		break;
	case 0x39:
		szString = "Space";
		break;
	case 0x3A:
		szString = "Caps Lock";
		break;
	case 0x47:
		szString = "Numpad 7";
		break;
	case 0x48:
		szString = "Numpad 8";
		break;
	case 0x49:
		szString = "Numpad 9";
		break;
	case 0x4A:
		szString = "Numpad -";
		break;
	case 0x4B:
		szString = "Numpad 4";
		break;
	case 0x4C:
		szString = "Numpad 5";
		break;
	case 0x4D:
		szString = "Numpad 6";
		break;
	case 0x4E:
		szString = "Numpad +";
		break;
	case 0x4F:
		szString = "Numpad 1";
		break;
	case 0x50:
		szString = "Numpad 2";
		break;
	case 0x51:
		szString = "Numpad 3";
		break;
	case 0x52:
		szString = "Numpad 0";
		break;
	case 0x53:
		szString = "Numpad .";
		break;
	case 0xC8:
		szString = "Up Arrow";
		break;
	case 0xD0:
		szString = "Down Arrow";
		break;
	case 0xCB:
		szString = "Left Arrow";
		break;
	case 0xCD:
		szString = "Right Arrow";
		break;	
	default:
		szString = "Not a Valid Key";
		break;
	}
#pragma endregion

	return szString;

}