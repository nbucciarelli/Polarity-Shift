//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"optionsState.cpp"
//	Author:			Jared Hamby (JH)
//	Purpose:		Handles the options state
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "optionsState.h"
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

#include <fstream>
using std::ifstream;
using std::ofstream;
//#include "..\..\Engines\CParticleEffectManager.h"
//
optionsState::optionsState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");

	menuItemString = new char*[TOTAL];

	//menuItemString[SFX] = "SFX: 0";
	//menuItemString[MUSIC] = "Music: 0";
	menuItemString[KEYBINDINGS] = "Keybindings";
	menuItemString[SAVE] = "Save";
	menuItemString[LOAD] = "Load";
	menuItemString[BACK] = "Back";
	menuLast = BACK;
}

optionsState::~optionsState(void)
{
	viewManager::getInstance()->releaseTexture(foregroundID);
}

optionsState* optionsState::getInstance()
{
	static optionsState Elgoog;
	return &Elgoog;
}

void optionsState::enter(void)
{

	//m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_Test4.prt");
	//CParticleEffectManager::GetInstance()->Play(m_nParticleImageID, true);
	m_fSoundLerp = 100;
	m_fXLerp = 1024;
	m_bMainMenu = false;
	m_bKeyBind = false;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	menuState::enter();
}

void optionsState::exit(void)
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	m_fSoundLerp = 100;
	menuState::exit();
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
}
void optionsState::update(float dt)
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
		else if(m_bKeyBind == true)
			EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_KEYBINDINGS));

	}
}

bool optionsState::input(float dt)
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
		}
		else if(theInput->KeyPressed(DIK_UP))
		{
			if(menuPos > 0)
				menuPos--;
			else
				menuPos = menuLast;
		}
		else if(theInput->KeyPressed(DIK_LEFT)  || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
		{
			if(menuPos == SFX)
			{
				game::GetInstance()->SetSFXLevel(game::GetInstance()->GetSFXLevel() - 5);
				if(game::GetInstance()->GetSFXLevel() <= 0)
					game::GetInstance()->SetSFXLevel(0);
				CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			}
			else if(menuPos == MUSIC)
			{
				game::GetInstance()->SetMusicLevel(game::GetInstance()->GetMusicLevel() - 5);
				if(game::GetInstance()->GetMusicLevel() <= 0)
					game::GetInstance()->SetMusicLevel(0);
			}
			else if(menuPos == FREQ)
			{
				game::GetInstance()->SetFreqLevel(game::GetInstance()->GetFreqLevel() - 5000);
				if(game::GetInstance()->GetFreqLevel() <= 0)
					game::GetInstance()->SetFreqLevel(0);
				CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			}
		}
		else if(theInput->KeyPressed(DIK_RIGHT) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			if(menuPos == SFX)
			{
				game::GetInstance()->SetSFXLevel(game::GetInstance()->GetSFXLevel() + 5);
				if(game::GetInstance()->GetSFXLevel() >= 100)
					game::GetInstance()->SetSFXLevel(100);
				CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			}
			else if(menuPos == MUSIC)
			{
				game::GetInstance()->SetMusicLevel(game::GetInstance()->GetMusicLevel() + 5);
				if(game::GetInstance()->GetMusicLevel() >= 100)
					game::GetInstance()->SetMusicLevel(100);
			}
			else if(menuPos == FREQ)
			{
				game::GetInstance()->SetFreqLevel(game::GetInstance()->GetFreqLevel() + 5000);
				if(game::GetInstance()->GetFreqLevel() >= 100000)
					game::GetInstance()->SetFreqLevel(100000);
				CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetSZSCHHHSound());
			}
		}
	}
	menuState::input(dt);

	return true;
}

void optionsState::menuHandler()
{
	switch(menuPos)
	{
	case SFX:
	case MUSIC:
	case FREQ:
		break;
	case KEYBINDINGS:
		m_bIsExiting = true;
		m_bKeyBind = true;
		break;
	case SAVE:
		{
			ofstream ofl;
			ofl.open("Resource//SavedGames//game.sav");
			if (ofl.is_open())
			{
				game* gameinstance = game::GetInstance();
				ofl << 	gameinstance->GetTutorialDone();
				bool* levelsdone = gameinstance->GetLevelComplete();
				for (int i = 0 ; i < NUMLEVELS ; ++i)
					ofl << levelsdone[i];
				bool* achievementsunlocked = gameinstance->GetAchievementUnlock();
				for (int i = 0 ; i < NUMLEVELS ; ++i)
					ofl << achievementsunlocked[i];				
			}
			break;
			ofl.close();
		}
	case LOAD:
		{		
			ifstream fin;
			fin.open("Resource//SavedGames//game.sav");
			if (fin.is_open())
			{
				game* gameinstance = game::GetInstance();
				char buffer;
				fin >> buffer;
				//bool buff;
				//buff = (bool)buffer;
				gameinstance->SetTutorialDone(buffer != 0);
				char levelsdone[10];
				for (int i = 0 ; i < NUMLEVELS ; ++i)
				{
					fin >> levelsdone[i];
  					if (levelsdone[i] == '1')
  						gameinstance->SetLevelComplete(i-1);															
				}
				char achievementsunlocked[10];
				for (int i = 0 ; i < NUMLEVELS ; ++i)
				{
					fin >> achievementsunlocked[i];
					if (achievementsunlocked[i] == '1')
						gameinstance->SetAchievementUnlock(i);					
				}
				
			}
			fin.close();
			break;
		}
	case BACK:
		m_bIsExiting = true;
		m_bMainMenu = true;
		break;
	}
}

void optionsState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));

	theFont->drawText("Options", (int)(383 + m_fXLerp), 65, textColor, 1);

	char buffer[32];
	sprintf_s(buffer, "SFX: %d", (game::GetInstance()->GetSFXLevel()));
	if(menuPos != SFX)
		theFont->drawText(buffer, (int)(20 + m_fXLerp + xPos), yPos, textColor);
	else
		theFont->drawText(buffer, (int)(20 + m_fXLerp + xPos), yPos, highlightColor);


	char buffer2[32];
	sprintf_s(buffer2, "Music: %d", (game::GetInstance()->GetMusicLevel()));
	if(menuPos != MUSIC)
		theFont->drawText(buffer2, (int)(20 + m_fXLerp + xPos), yPos + 60, textColor);
	else
		theFont->drawText(buffer2, (int)(20 + m_fXLerp + xPos), yPos + 60, highlightColor);

	char buffer3[32];
	int freq = (int)(game::GetInstance()->GetFreqLevel()/500.0f);
	sprintf_s(buffer3, "Frequency: %d", freq);
	if(menuPos != FREQ)
		theFont->drawText(buffer3, (int)(20 + m_fXLerp + xPos), yPos + 120, textColor);
	else
		theFont->drawText(buffer3, (int)(20 + m_fXLerp + xPos), yPos + 120, highlightColor);

	//Draw menu items
	for(int c = 3; c < menuLast+1; c++)
		if(c != menuPos)
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 60, textColor);
		else //For the selected item, use highlight color
			theFont->drawText(menuItemString[c], (int)(20 + m_fXLerp + xPos), yPos + c * 60, highlightColor);

	//Draw meun cursor at the selected item
	//viewManager::getInstance()->drawTexture(cursorID,
	//	&vector3(float(xPos-70), float(yPos-20 + menuPos * 50), 0));

	//menuState::render();
	//CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetEmitterPosX(), menuState::GetEmitterPosY()); 
}