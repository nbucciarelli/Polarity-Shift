#include "CHighScoresState.h"
#include "menustate.h"

#include "..\..\Wrappers\CSGD_DirectInput.h"
#include "..\..\EventSystem\eventManager.h"
#include "..\..\EventSystem\globalEvents.h"
#include "ctime"
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
#include "..\..\XMLParser\irrXML.h"
using namespace irr;
using namespace io;

namespace
{
#define numScores 10
#define _FileName "Resource/highscores.xml"
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
	menuItemString = new char*[TOTAL];
	menuLast = EXIT;

	menuItemString[EXIT] = "Exit";
}

bool CHighScoresState::input(float delta)
{
	if(theInput->KeyPressed(DIK_ESCAPE))
	{
		EM->sendGlobalEvent(GE_STATE_POP);
	}
		
	return menuState::input(delta);
	
}

void CHighScoresState::render() const
{
	menuState::render();
	//mymap::const_reverse_iterator itr = score.rbegin();
	char s[100];
	//for(int i = 0; i < numScores; ++i, ++itr)
	for(mymap::const_reverse_iterator itr = score.rbegin();
		itr != score.rend(); itr++)
	{
		sprintf_s(s, "%i -- %s", itr->first, itr->second->timeStamp.c_str());
		//ViewManager::GetInstance().RenderText(300, (float)(i+1) * 20 , s, Color(255,255,255,255));
	}
}

void CHighScoresState::enter()
{
	menuState::enter();
	EM = eventManager::getInstance();
	Load();
}

void CHighScoresState::exit()
{
	reset();
	menuState::exit();
}

void CHighScoresState::update(float delta)
{
	
}

void CHighScoresState::menuHandler(void)
{
	switch(menuPos)
	{
	case EXIT:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_MAINMENU));
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

		__int64 ltime = time(NULL);	/* calendar time */	/* get current cal time */ // __int64 = time_t
		sprintf_s(s,"%s",asctime(localtime(&ltime)));
		score[nScore] = new structTStamp(s);	
		write();
		changed = true;
	}
	reset();
	return changed;
}