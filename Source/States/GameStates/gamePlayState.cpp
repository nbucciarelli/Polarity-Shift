////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “gamePlayState.cpp”
// Author: Scott Smallback (SS) / Jared Hamby (JH)
// Purpose: This is the code file for the game play state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "gamePlayState.h"
#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\eventIDs.h"
#include "../../EventSystem/gameEvent.h"
#include "../../EventSystem/playHandler.h"
#include "../../Objects/objManager.h"
#include "..\..\Engines\CTileEngine.h"
#include "..\..\Engines\CAIEngine.h"
#include "..\..\game.h"
#include "levelChooseState.h"
#include "../../helpers/datatypes.h"
#include "../../wrappers/mouse.h"
#include "../../Objects/playerObj.h"
#include "../../Engines/CParticleEffectManager.h"
#include "..\..\Helpers\bitFont.h"
#include "../../engines/debugControl.h"
#include "CKeyState.h"
#include "..\GamePlayStates\CTallySheet.h"
#include "../../EventSystem\globalEvents.h"
#include "../../Wrappers/CSGD_FModManager.h"
#include "..\..\Helpers\CXBOXController.h"



gamePlayState::gamePlayState() : debugger(0) {timeMod = 2.0f;}
gamePlayState::~gamePlayState() {}

gamePlayState* gamePlayState::getInstance()
{
	static gamePlayState playa;

	return &playa;
}

void gamePlayState::enter(void)
{
	timeMod = 2.0f;
	//for (int i = 0 ; i < 4 ; ++i)
	//	m_bIsBuffered[i] = true;
	m_cLeftRight = '0';
	m_cRightTrigger = '0';
	m_cLeftTrigger = '0';

	m_fInvulnerableTime = 0.0f;

	theMouse = mouse::getInstance();

	OM = objManager::getInstance();
	theInput = CSGD_DirectInput::GetInstance();
	EM = eventManager::getInstance();
	theFont = bitFont::getInstance();
	Player1 = new CXBOXController(1);
	TE = CTileEngine::GetInstance();

	m_fLevelTime = 0;
	m_fLevelScore = 100;
	
	for (int i = 0 ; i < 4 ; ++i)
	{
		Tutorials[i] = false;
	}

	//string buff;
	char buff[256] = {0};
	sprintf_s(buff,256,"Resource/PS_Level%d.bmf",levelChooseState::getInstance()->GetPlayLevel());
	TE->LoadMap(string(buff));

	m_nParticleImageID = CParticleEffectManager::GetInstance()->LoadEffect("Resource/PS_SmokeBottomRight.prt");

	m_bTrapActive = false;

	handler = new playHandler;
	handler->initialize();

	for(int c = BEGIN_PLAY_EVENTS; c < END_PLAY_EVENTS; c++)
		EM->registerClient(c, this);

	EM->sendEvent(EVENT_GAMELOADING);

	AIE = new CAIEngine();
}

void gamePlayState::exit(void)
{
	while(rendering)
		Sleep(1);

	delete Player1;
	gameState::exit();

	if(debugger)
	{
		debugger->shutdown();
		delete debugger;
		debugger = 0;
	}

	m_bTrapActive = false;

	TE->ShutDown();
	delete AIE;
	OM->clear();
	handler->shutdown();
	delete handler;

	theMouse->shutdown();
	CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);

}

bool gamePlayState::input(float dt)
{
	if (theInput->KeyPressed(DIK_ESCAPE) || theInput->KeyPressed(DIK_F10))
		EM->sendEvent(EVENT_GAMEPAUSE);

	if(entered)
	{
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			m_cLeftRight = '1';
		else if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			m_cLeftRight = '2';
		else
			m_cLeftRight = '0';

		if(theInput->KeyDown(game::GetInstance()->GetKeys().m_nRunLeft) || m_cLeftRight == '1')
			EM->sendEvent(EVENT_PLAYERGOLEFT);
		else if(theInput->KeyDown(game::GetInstance()->GetKeys().m_nRunRight) || m_cLeftRight == '2')
			EM->sendEvent(EVENT_PLAYERGORIGHT);
		else if((theInput->KeyReleased(game::GetInstance()->GetKeys().m_nRunLeft)
			|| theInput->KeyReleased(game::GetInstance()->GetKeys().m_nRunRight)) || m_cLeftRight == '0')
			EM->sendEvent(EVENT_PLAYERSTOP);

		if(theInput->KeyDown(game::GetInstance()->GetKeys().m_nJump) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)|| (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)|| (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			Tutorials[0] = true;
			EM->sendEvent(EVENT_PLAYERJUMP);
		}
		
		if(theInput->KeyReleased(game::GetInstance()->GetKeys().m_nJump))
			EM->sendEvent(EVENT_PLAYERJUMPSTOP);

	}

	/*if (Player1->GetState().Gamepad.bLeftTrigger)
		m_cLeftTrigger = '1';
	else 
		m_cLeftTrigger = '0';

	if (Player1->GetState().Gamepad.bRightTrigger)
		m_cRightTrigger = '1';
	else
		m_cRightTrigger = '0';*/

	if(!Player1->GetState().Gamepad.bLeftTrigger && m_cLeftTrigger == '1')
		m_cLeftTrigger = '2';
	else if(Player1->GetState().Gamepad.bLeftTrigger)
		m_cLeftTrigger = '1';
	else
		m_cLeftTrigger = '0';

	if(!Player1->GetState().Gamepad.bRightTrigger && m_cRightTrigger == '1')
		m_cRightTrigger = '2';
	else if (Player1->GetState().Gamepad.bRightTrigger)
		m_cRightTrigger = '1';
	else
		m_cRightTrigger = '0';

	float pos = Player1->GetState().Gamepad.sThumbRX;
	if(fabs(pos) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		if(Player1->GetState().Gamepad.sThumbRX < 0)
		{
			float mod = Player1->GetState().Gamepad.sThumbRX/40.0f;
			mouse::getInstance()->setPos(mouse::getInstance()->getPos()+vector3(mod*dt,0,0));
		}
		else
		{
			float mod = Player1->GetState().Gamepad.sThumbRX/40.0f;
			mouse::getInstance()->setPos(mouse::getInstance()->getPos()+vector3(mod*dt,0,0));
		}

	}
	pos = Player1->GetState().Gamepad.sThumbRY;
	if(fabs(pos) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		if(Player1->GetState().Gamepad.sThumbRY < 0)
		{
			float mod = Player1->GetState().Gamepad.sThumbRY/40.0f;
			mouse::getInstance()->setPos(mouse::getInstance()->getPos()+vector3(0,-mod*dt,0));
		}
		else
		{
			float mod = Player1->GetState().Gamepad.sThumbRY/40.0f;
			mouse::getInstance()->setPos(mouse::getInstance()->getPos()+vector3(0,-mod*dt,0));
		}

	}	

	if(theInput->MouseButtonPressedEx(0) || m_cLeftTrigger == '1')
	{
		Tutorials[1] = true;
		EM->sendEvent(EVENT_PLAYERFIRE);
		if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound1()))
			CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetGunSound1());
	}

	if(theInput->MouseButtonPressedEx(1) || m_cRightTrigger == '1')
	{
		EM->sendEvent(EVENT_PLAYERFIRE2);
		if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(game::GetInstance()->GetGunSound2()))
			CSGD_FModManager::GetInstance()->PlaySound(game::GetInstance()->GetGunSound2());
	}

	if(theInput->MouseButtonReleased(0) || m_cLeftTrigger == '2')
	{
		EM->sendEvent(EVENT_PLAYERCEASEFIRE, new int(EVENT_PLAYERFIRE));
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound1());
	}

	if(theInput->MouseButtonReleased(1) || m_cRightTrigger == '2')
	{
		EM->sendEvent(EVENT_PLAYERCEASEFIRE, new int(EVENT_PLAYERFIRE2));
		CSGD_FModManager::GetInstance()->StopSound(game::GetInstance()->GetGunSound2());
	}


	if(theInput->KeyPressed(DIK_DELETE))
		EM->sendEvent(EVENT_DEBUG_SWITCH);
	if(theInput->KeyPressed(DIK_NUMPADPLUS))
		timeMod+=.5f;
	if(theInput->KeyPressed(DIK_NUMPADMINUS))
		timeMod-=.5f;

	// 	if (theInput->KeyPressed(DIK_P))
	// 	{
	// 		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_TALLYSHEET));
	// 		CTallySheetState::getInstance()->Initialize(/*LevelNum*/1 ,100.0f, m_fLevelTime);
	// 	}
	if (theInput->CheckBufferedKeysEx()) 
	{ 
		m_qCheatCode.push(theInput->CheckBufferedKeysEx()); 
		if (m_qCheatCode.size() > 5) 
		{ 
			m_qCheatCode.pop(); 
		}  
	}

	return true;
}

void gamePlayState::update(float dt)
{
	m_fLevelTime += dt;

	if (m_bIsGodMode)
	{
		m_fInvulnerableTime += dt;
		if (m_bIsGodMode && m_fInvulnerableTime >= 5.0f)
		{
			m_bIsGodMode = false;
		}

	}
	

	dt*=timeMod;
	if(entered)
	{
		AIE->update(dt);

		OM->checkCollisions();

		OM->update(dt);

		theMouse->update(dt);

		CParticleEffectManager::GetInstance()->Update(dt);

		if(debugger)
			debugger->update(dt/timeMod);
	}
		

	EM->processEvents();

	if (m_qCheatCode.size() == 5) 
	{ 
		if(m_qCheatCode.front() == 'u') 
		{ 
			m_qCheatCode.pop(); 
			if (m_qCheatCode.front() == 'd') 
			{ 
				m_qCheatCode.pop(); 
				if (m_qCheatCode.front() == 'i') 
				{ 
					m_qCheatCode.pop(); 
					if (m_qCheatCode.front() == 'e') 
					{ 
						m_qCheatCode.pop(); 
						if(m_qCheatCode.front() = 'd')
						{
							m_qCheatCode.pop();
							SetGodMode(true);
						}
					} 
				} 
			} 
		}
	}

	if (m_qCheatCode.size() == 5) 
	{ 
		if(m_qCheatCode.front() == 'l') 
		{ 
			m_qCheatCode.pop(); 
			if (m_qCheatCode.front() == 'e') 
			{ 
				m_qCheatCode.pop(); 
				if (m_qCheatCode.front() == 'v') 
				{ 
					m_qCheatCode.pop(); 
					if (m_qCheatCode.front() == 'e') 
					{ 
						m_qCheatCode.pop(); 
						if(m_qCheatCode.front() = 'l')
						{
							m_qCheatCode.pop();
							for(int i = 0; i < 10; i++)
								game::GetInstance()->SetLevelComplete(i);
						}
					} 
				} 
			} 
		}
	}
}

void gamePlayState::render(void) const
{
	if(!entered)
		return;


	OM->render();

	if(TE)
		TE->Render();

	theMouse->render();

	char buff[256] = {0};
	sprintf_s(buff,256,"Level: %d",levelChooseState::getInstance()->GetPlayLevel());
	theFont->drawText(buff, 200, 30, 0xffff0000, .5f);

	sprintf_s(buff, 256, "Time: %.2f", m_fLevelTime);
	theFont->drawText(buff, 1600, 30, 0xffff0000, .5f);

	if(m_bTrapActive == true)
	{
		CParticleEffectManager::GetInstance()->Render(m_nParticleImageID,TE->GetTraps()[0].x,TE->GetTraps()[0].y);
	}

	if (game::GetInstance()->GetTutorialDone() == false)
	{
		if (Tutorials[0] == false && AIE->GetTrackPos()->x >= 100)
		{
			char buffertut[256] = {0};
			sprintf_s(buffertut, 256, "Press %s to jump over the walls", CKeyState::getInstance()->SetKeyString(game::GetInstance()->GetKeys().m_nJump));
			theFont->drawText(buffertut, 100, 700, 0xffff0000);
						
		}
		else if (Tutorials[1] == false && AIE->GetTrackPos()->x >= 250)
		{
			//char buffertut[256] = {0};
			//sprintf_s(buffertut, 256, "Press %s to push the blocks onto each other", "Mouse 1");
			theFont->drawText("Press Mouse 1 to push", 100, 700, 0xffff0000);
			theFont->drawText("the blocks onto each other", 100, 750, 0xffff0000);
		}
		else if (Tutorials[2] == false && AIE->GetTrackPos()->x >= 600)
		{
			theFont->drawText("Run through the exit!", 100, 700, 0xffff0000);
		}
	}

	if(debugger)
		debugger->render();

}

void gamePlayState::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case EVENT_LEVELLOADED:
		entered = true;
		break;
	case EVENT_LEVELFINISHED:
		{

			game::GetInstance()->SetTutorialDone(true);
			m_fLevelScore = m_fLevelScore - (m_fLevelTime * 5);
			if (m_fLevelScore <= 0.0f)
				m_fLevelScore = 0.0f;
			int currlevel = levelChooseState::getInstance()->GetPlayLevel();
			game::GetInstance()->SetLevelComplete(currlevel-1);
			if(m_fLevelTime <= game::GetInstance()->GetAchievementNumbers(currlevel-1))
				game::GetInstance()->SetAchievementUnlock(currlevel-1);
			if (currlevel < 10)
				EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_TALLYSHEET));
			else
			{
				game::GetInstance()->Reset();
				EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_WINSTATE));
				
			}
			
			
			CTallySheetState::getInstance()->Initialize(currlevel,m_fLevelScore, m_fLevelTime);

			entered = false;
		}
		break;
	case EVENT_DEBUG_SWITCH:
		if(!debugger)
		{
			debugger = new debugControl;
			debugger->initialize();
		}
		else
		{
			debugger->shutdown();
			delete debugger;
			debugger = 0;
		}
		break;
	}
}