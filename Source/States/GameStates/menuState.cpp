//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"menuState.cpp"
//	Author:			Scott Smallback (SS) / Nick Bucciarelli (NB) / Jared Hamby (JH)
//	Purpose:		handles the menus
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "menuState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\Objects\objManager.h"
#include "..\..\Wrappers\viewManager.h"
#include "..\..\Objects\baseObj.h"
#include "..\..\game.h"
#include "..\..\Helpers\datatypes.h"
#include "..\..\Helpers\physics.h"
#include "..\..\Helpers\bitFont.h"
#include "../../EventSystem/eventManager.h"
#include "..\..\Helpers\CXBOXController.h"
#include "../../Helpers/SGD_Math.h"

#define CURSOR "Resource/PS_menuCursor.png"

menuState::menuState(int x, int y, unsigned int color, unsigned int highlight)
: xPos(x), yPos(y), textColor(color), highlightColor(highlight), menuItemString(0)
{
	cursorID = viewManager::getInstance()->loadTexture(CURSOR);
}

menuState::~menuState()
{
	viewManager::getInstance()->releaseTexture(cursorID);

	if(menuItemString)
		delete[] menuItemString;
}

void menuState::enter(void)
{
	highlightColor = 0xffa4a4ff;
	theInput = inputDevice::GetInstance();
	theFont = bitFont::getInstance();
	objM = objManager::getInstance();
	EM = eventManager::getInstance(); 
	Player1 = new CXBOXController(1);
	m_bIsBuffered = true;
	m_nEmitterPosX = 500;
	m_nEmitterPosY = 500;

	menuPos = 0;
	
	gameState::enter();
}

void menuState::exit(void)
{
	delete Player1;

	gameState::exit();
}

bool menuState::input(float dt)
{
	if(m_bIsMoving == false)
	{
		if (theInput->KeyPressed(DIK_RETURN))
		{
			
			highlightColor = 0xffa4a4ff;
			menuHandler();
		}
		else if (theInput->KeyPressed(DIK_DOWN))
		{
			if(menuPos < menuLast)
				menuPos++;
			else
				menuPos = 0;
		}else if(theInput->KeyPressed(DIK_UP))
		{
			if(menuPos > 0)
				menuPos--;
			else
				menuPos = menuLast;
		}
	}


	if (Player1->IsConnected())
	{
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			if (m_bIsBuffered == true)
			{
				Player1->Vibrate(65535, 65535);

				if(menuPos > 0)
					menuPos--;
				else
					menuPos = menuLast;

				m_bIsBuffered = false;
			}

		}else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN )
		{
			if (m_bIsBuffered == true)
			{
				Player1->Vibrate(65535, 65535);
				if(menuPos < menuLast)
					menuPos++;
				else
					menuPos = 0;

				m_bIsBuffered = false;
			}

		}else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A )
		{
			if (m_bIsBuffered == true)
			{
				menuHandler();
				Player1->Vibrate(65535, 65535);
				m_bIsBuffered = false;
			}

		}else
		{
			m_bIsBuffered = true;
			Player1->Vibrate(0, 0);
		}
	}
	highlightColor = 0xffa4a4ff;
	

#pragma region "Thumb Sticks"
	//float leftThumbY = Player1->GetState().Gamepad.sThumbLX;

	/*
	if (Player1->GetState().Gamepad.sThumbLX)
	{
	m_nEmitterPosX += (Player1->GetState().Gamepad.sThumbLX / 500000);
	}

	if (Player1->GetState().Gamepad.sThumbLY)
	{
	m_nEmitterPosY += (Player1->GetState().Gamepad.sThumbLY / 500000);
	}

	m_fUpdateTimer -= dt;
	if (m_fUpdateTimer > 0.0f)
	return true;
	else
	{

	m_fUpdateTimer = .016f;
	//m_nEmitterPosX += Player1->GetState().Gamepad.sThumbLX % 1000;
	//m_nEmitterPosY += Player1->GetState().Gamepad.sThumbLY % 1000;
	}*/
#pragma endregion


	return true;
}

void menuState::update(float dt)
{
			
}

void menuState::render(void) const
{
	if(!menuItemString)
		return;

	//Draw menu items
	for(int c = 0; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], xPos, yPos + c * 50, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], xPos, yPos + c * 50, highlightColor);

	//Draw meun cursor at the selected item
	viewManager::getInstance()->drawTexture(cursorID,
		&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	
}
