///////////////////////////////////////////////////
//	Function: CVideoMaster.h
//
//	Full Sail Real World Education
//
//	Agoraphobic Games
//
//	Purpose: to handle and manage all the videos
//
//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial 
//
////////////////////////////////////////////////////////////////////
#pragma once
#define _VIDEO_
#ifdef _VIDEO_

//#include "AG_def.h"
#include <windows.h>
// first you need the windows SDK dowloaded and included
//headers and libs required for direct show
#include <dshow.h>

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
//#pragma comment(lib, "Strmbase.lib")

#include <vector>
#include <string>
using std::string;
using std::vector;

#define int32 int
#define uint32 unsigned

// windows message sent by direct show you need to catch this in winmain
#define WM_GRAPHNOTIFY WM_APP + 1

// main class
class CVideoMaster
{
private:
	// structure for all the info about the video
	struct tVid
	{
		// LPCWSTR for the file name it needs to be this for direct show
		LPCWSTR m_szFileName;
		int32 m_nRef;
		bool m_bPlaying, m_bStop, m_bPaused;

		tVid()
		{
			m_szFileName = L"\0";
			m_nRef = 0;
			m_bPlaying = false;
			m_bStop = false;
			m_bPaused = false;
		}
	};
	// direct show interfaces the HTML from the school has more info on this
	IGraphBuilder	*m_pGB;
	IMediaControl	*m_pMC;
	IMediaEventEx	*m_pME;
	IVideoWindow	*m_pVW;
	IMediaSeeking	*m_pMS;
	HWND			 m_hWnd;

	bool m_bPlaying;
	
	vector<tVid> m_vVideos;

	CVideoMaster() { m_bPlaying = false; 
	m_pGB = NULL;
	m_pMC = NULL;
	m_pME = NULL;	
	m_pVW = NULL;
	m_hWnd = NULL;
	
	
	
	
	
	}
	CVideoMaster(const CVideoMaster &);
	CVideoMaster* operator=(const CVideoMaster &);
	~CVideoMaster() {}
public:

	inline bool GetIsPlaying() { return m_bPlaying; }
	////////////////////////////////////////////////
	//	Function: GetInstance
	//
	//	Purpose: returns a single instance of the class
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	static CVideoMaster* GetInstance();

	//////////////////////////////////////////////////////////////
	//	Function: Init
	//
	//	Purpose: Initializes the all the direct show components 
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	//////////////////////////////////////////////////////////////
	void Init(HWND);

	////////////////////////////////////////////////
	//	Function: Play
	//
	//	Purpose: Plays the Video from the beginning
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void Play(int32 _ID, int _nWindowWidth, int _nWindowHeight, bool _bIsWindowed);

	////////////////////////////////////////////////
	//	Function: Shutdown
	//
	//	Purpose: Shuts down all the direct show components
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void Shutdown();

	////////////////////////////////////////////////
	//	Function: Load
	//
	//	Purpose: loads a video in from a file prefixe string with L 
	//	to convert to unicode
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	int32 Load(LPCWSTR);

	////////////////////////////////////////////////
	//	Function: HandleEvent
	//
	//	Purpose: handles the messages sent from the video
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void HandleEvent();

	////////////////////////////////////////////////
	//	Function: Stop
	//
	//	Purpose: stops the video
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void Stop();

	////////////////////////////////////////////////
	//	Function: Unload
	//
	//	Purpose: unloads the video
	//
	//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void UnLoad(int32 _nId);

	////////////////////////////////////////////////
	//	Function: Repaly
	//
	//	Purpose: Restarts and plays the video
	//
	//	Revisions: 08/26/08 - Steve Emmerich (SDE) - Initial
	//
	////////////////////////////////////////////////////
	void RePlay();


};
#endif
