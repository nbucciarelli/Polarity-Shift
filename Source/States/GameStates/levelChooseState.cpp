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
#include "..\..\Wrappers\CSGD_FModManager.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\Helpers\CXBOXController.h"
//#include "..\..\Engines\CParticleEffectManager.h"

levelChooseState::levelChooseState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");

	menuItemString = new char*[TOTAL];

	menuItemString[LEVEL1] = "Level 1";
	menuItemString[LEVEL2] = "Level 2";
	menuItemString[LEVEL3] = "Level 3";
	menuItemString[LEVEL4] = "Level 4";
	menuItemString[LEVEL5] = "Level 5";
	menuItemString[LEVEL6] = "Level 6";
	menuItemString[LEVEL7] = "Level 7";
	menuItemString[LEVEL8] = "Level 8";
	menuItemString[LEVEL9] = "Level 9";
	menuItemString[LEVEL10] = "Level 10";
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
	//m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_Test4.prt");
	//CParticleEffectManager::GetInstance()->Play(m_nParticleImageID, true);
	m_nSelectedLevel = 1;
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
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
}

void levelChooseState::menuHandler()
{
	switch(menuPos)
	{
	case LEVEL1:
		SetPlayLevel(1);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case LEVEL2:
		SetPlayLevel(2);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case LEVEL3:
		SetPlayLevel(3);
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_PLAY));
		break;
	case LEVEL4:
		SetPlayLevel(4);
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

	theFont->drawText("Level Select", (int)(288 + m_fXLerp), 65, textColor, 1);
	bool* bIsLevelComplete = game::GetInstance()->GetLevelComplete();
	//Draw menu items
	for(int c = 0; c < menuLast; c++)
	{
		if(c <= 4)
		{
			if(c != menuPos)
				if (bIsLevelComplete[c])
					theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 80, textColor);
				else
					theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 80, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128));

			else //For the selected item, use highlight color
				theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 80, highlightColor);
		}
		else
		{
			if(c != menuPos)
				if (bIsLevelComplete[c])
					theFont->drawText(menuItemString[c], (int)(400 + m_fXLerp + xPos), yPos + (c-5) * 80, textColor);
				else
					theFont->drawText(menuItemString[c], (int)(400 + m_fXLerp + xPos), yPos + (c-5) * 80, viewManager::getInstance()->color_argb((char)128, (char)128,(char)128,(char)128));

			else //For the selected item, use highlight color
				theFont->drawText(menuItemString[c], (int)(400 + m_fXLerp + xPos), yPos + (c-5) * 80, highlightColor);
		}

	}

	if(menuPos == menuLast)
		theFont->drawText(menuItemString[menuLast], (int)(20 + m_fXLerp + xPos), yPos + 5 * 80, highlightColor);
	else
		theFont->drawText(menuItemString[menuLast], (int)(20 + m_fXLerp + xPos), yPos + 5 * 80, textColor);


	//Draw meun cursor at the selected item
	//viewManager::getInstance()->drawTexture(cursorID,
	//	&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	//menuState::render();
	//CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetEmitterPosX(), menuState::GetEmitterPosY()); 
}

bool levelChooseState::input(float dt)
{
	bool* bIsLevelComplete = game::GetInstance()->GetLevelComplete();
	if(m_bIsMoving == false)
	{
		if (theInput->KeyPressed(DIK_RETURN))
		{

			highlightColor = 0xffa4a4ff;
			/*if (bIsLevelComplete[menuPos] || menuPos == menuLast)
			{
			menuHandler();
			}*/
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
		}else if (theInput->KeyPressed(DIK_RIGHT))
		{
			if (menuPos != menuLast)
			{

				if (menuPos>=0 && menuPos <= 4)
					menuPos += 5;
				else if (menuPos > 4 && menuPos <= 9)
					menuPos -= 5;

			}
		}else if (theInput->KeyPressed(DIK_LEFT))
		{
			if (menuPos != menuLast)
			{

				if (menuPos>=0 && menuPos <= 4)
					menuPos += 5;
				else if (menuPos > 4 && menuPos <= 9)
					menuPos -= 5;

			}

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
				/*if (bIsLevelComplete[menuPos] || menuPos == menuLast)
				{
				menuHandler();
				}*/
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

	return true;


}