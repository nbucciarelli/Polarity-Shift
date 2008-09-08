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
//#include "..\..\Engines\CParticleEffectManager.h"

optionsState::optionsState(void)
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_tempmenu.bmp", D3DCOLOR_XRGB(255, 0, 255));

	menuItemString = new char*[TOTAL];

	menuItemString[SFX] = "SFX: 0";
	menuItemString[MUSIC] = "Music: 0";
	menuItemString[KEYBINDINGS] = "Keybindings";
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
	m_fXPer = 0;
	m_fXLerp = 0;
	menuState::enter();
}

void optionsState::exit(void)
{
	menuState::exit();
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
	//CParticleEffectManager::GetInstance()->Unload(m_nParticleImageID);
}
void optionsState::update(float dt)
{
	if(!entered)
		return;

	m_fTime += dt; 
	if (dt >= .016f) 
	{ 
		m_fXPer += .02f; 
		m_fXLerp = Lerp(1024, 0, m_fXPer); 
		if(m_fXPer >= 1)
			m_fXPer = 1;
	}
}

void optionsState::menuHandler()
{
	switch(menuPos)
	{
	case SFX:
	case MUSIC:
		break;
	case KEYBINDINGS:
		break;
	case BACK:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
		break;
	}
}

void optionsState::render(void) const
{
	if(!entered)
		return;

	viewManager::getInstance()->drawTexture(foregroundID, &vector3(m_fXLerp, 0, 0));

	menuState::render();
	//CParticleEffectManager::GetInstance()->Render(m_nParticleImageID, menuState::GetEmitterPosX(), menuState::GetEmitterPosY()); 
}