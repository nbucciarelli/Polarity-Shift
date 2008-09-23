#include "CHighScoresState.h"
#include "menustate.h"

#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "..\..\Helpers\bitFont.h"
#include "ctime"
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
#include "..\..\XMLParser\irrXML.h"
#include "..\..\Wrappers\viewManager.h"
#include "..\..\Wrappers\CSGD_FModManager.h"
#include "..\..\Helpers\SGD_Math.h"
#include "..\..\game.h"
using namespace irr;
using namespace io;

namespace
{
#define numScores 10
#define _FileName "Resource/PS_highscores.xml"
	struct structTStamp 
	{
		std::string timeStamp;
		structTStamp(std::string s): timeStamp(s){}
	};
	typedef std::map<unsigned, structTStamp*> mymap;
	void Load();
	mymap score;
	bool Check(unsigned , unsigned);
	void reset()
	{
		for (mymap::iterator itr = score.begin(); itr != score.end(); ++itr)
		{
			delete itr->second;
		}
		score.clear();
	}
	void Load()
	{
		IrrXMLReader* xml = createIrrXMLReader(_FileName);
		for(int i = 0; xml && xml->read() && i < 10; ++i)
		{
			if(EXN_ELEMENT == xml->getNodeType())
			{
				if(!strcmp("TopTen", xml->getNodeName()))
				{
					score[xml->getAttributeValueAsInt("score")] = new structTStamp(xml->getAttributeValue("timestamp"));
				}
			}
		}
		delete xml;
	}
	void write()
	{
		char s[255];
		ofstream fout(_FileName);
		mymap::reverse_iterator itr = score.rbegin();
		for(int i = 0; i < numScores; ++itr, ++i)
		{
			sprintf_s(s,"<TopTen position=\"%d\" score=\"%d\" timestamp=\"%s\"/>\n",i + 1,itr->first,itr->second->timeStamp.c_str());
			fout << s;
		}
		fout.close();
	}
}

CHighScoresState::CHighScoresState()
{
	foregroundID = viewManager::getInstance()->loadTexture("Resource/Images/PS_Menu.png");
	menuItemString = new char*[TOTAL];
	menuLast = EXIT;

	menuItemString[EXIT] = "Exit";
}

bool CHighScoresState::input(float delta)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		m_bIsExiting = true;
	}
	
		
	return menuState::input(delta);
	
}

void CHighScoresState::render() const
{
	//menuState::render();
	viewManager::getInstance()->drawTexture(foregroundID, &vector3(20 + m_fXLerp, 0, 0));
	theFont->drawText("High Scores", (int)(313 + m_fXLerp), 65, textColor, 1);
	//mymap::const_reverse_iterator itr = score.rbegin();
	char s[100];
	int i = 0;
	//for(int i = 0; i < numScores; ++i, ++itr)
	for(mymap::const_reverse_iterator itr = score.rbegin();
		itr != score.rend(); itr++)
	{

		sprintf_s(s, "%i -- %s", itr->first, itr->second->timeStamp.c_str());
		//ViewManager::GetInstance().RenderText(300, (float)(i+1) * 20 , s, Color(255,255,255,255));
		//theFont->drawText(s,(float)(i+1) * 20, 20, 0xffff0000 );
		bitFont::getInstance()->drawText(s, (int)(m_fXLerp + 20), (int)(i+7) * 40, 0xffffffff);
		i++;
	}
}

void CHighScoresState::enter()
{
	menuState::enter();
	m_fXLerp = 1024;
	m_bIsMoving = true;
	m_bIsExiting = false;
	m_bIsExited = false;
	EM = eventManager::getInstance();
	Load();
}

void CHighScoresState::exit()
{
	m_bIsMoving = true;
	m_fXLerp = 1024;
	reset();
	menuState::exit();
}

void CHighScoresState::update(float delta)
{
	if(!entered)
		return;

	m_fTime += delta; 
	if(m_bIsMoving == true)
	{
		if (delta >= .016f) 
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
		if (delta >= .016f) 
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

void CHighScoresState::menuHandler(void)
{
	switch(menuPos)
	{
	case EXIT:
		m_bIsExiting = true;
		break;
	}
}

bool CHighScoresState::AddHighScore(unsigned int nScore)
{
	bool changed = false;
	Load();
	mymap::iterator itr = score.begin();
	if (itr->first < nScore)
	{
		char s[64];
		tm TempTime;
		localtime_s(&TempTime, NULL);
		//__int64 ltime = time(NULL);	/* calendar time */	/* get current cal time */ // __int64 = time_t
		sprintf_s(s,"%s",asctime_s(s,sizeof(TempTime),&TempTime));
		
		//sprintf_s(s,"%s",asctime_s(s,sizeof(localtime_s(&TempTime,&ltime)),(tm*)localtime_s(&TempTime,&ltime)));
		score[nScore] = new structTStamp(s);	
		write();
		changed = true;
	}
	reset();
	return changed;
}