///////////////////////////////////////////////////
//	Function: CVideoMaster.cpp
//
//	Full Sail Real World Education
//
//	Purpose: to handle and manage all the videos
//
//	Revisions: 08/19/08 - Steve Emmerich (SDE) - Initial 
//			   09/11/08 - Nick Bucciarelli (NB)
////////////////////////////////////////////////////////////////////

#include "CVideoMaster.h"

#ifdef _VIDEO_

CVideoMaster* CVideoMaster::GetInstance()
{
	static CVideoMaster Instance;
	return &Instance;
}

void CVideoMaster::Init(HWND _hWnd)
{
	// you must call this function to initialize DS
	CoInitialize(NULL);
	m_hWnd = _hWnd;

	// this initializeses the graph 
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void**) &m_pGB);

	// initialize all the other interfaces
	m_pGB->QueryInterface(IID_IMediaControl, (void **) &m_pMC);
	m_pGB->QueryInterface(IID_IVideoWindow, (void**) &m_pVW);
	m_pGB->QueryInterface(IID_IMediaEvent,	 (void **) &m_pME);
	m_pGB->QueryInterface(IID_IMediaSeeking, (void**) &m_pMS);
		
}

int CVideoMaster::Load(LPCWSTR _szFileName)
{

	// if there is no filename return -1
	if (!_szFileName)
		return -1;

	
	int id = -1;
	for(unsigned i = 0; i < m_vVideos.size(); ++i)
	{
		// check if that one is all ready loaded if it is return that index else set the id equal to i
		if ( _szFileName == m_vVideos[i].m_szFileName)
		{
			m_vVideos[i].m_nRef++;
			return i;
		}
		else if (m_vVideos[i].m_nRef == 0)
		{
			id = i;
			break;
			
		}
	}
	if ( id == -1)
	{
		//we don't have it loaded yet so lets make it and load it in
		tVid video;

		
		
		video.m_szFileName = _szFileName;

		video.m_nRef = 1;
		m_vVideos.push_back(video);

		return (int)m_vVideos.size() -1;

	}
	else return -1;
}

void CVideoMaster::Play(int _ID, int _nWindowWidth, int _nWindowHeight, bool _bIsWindowed)
{
				
	// create a DWORD for the window style
	DWORD dwWindowStyleFlags = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	//if its full screen change the style
	if (!_bIsWindowed)
	{
		dwWindowStyleFlags |= WS_VISIBLE | WS_POPUP;
		ShowCursor(false);	// Stop showing the mouse cursor
	}

	  
	//if(!m_vVideos[_ID].m_bPlaying)
	// we have more then one video so we need to shut everything down and restart everything 
	if( m_vVideos.size() > 1)
	{
		Shutdown();
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void**) &m_pGB);

	m_pGB->QueryInterface(IID_IMediaControl, (void **) &m_pMC);
	m_pGB->QueryInterface(IID_IVideoWindow, (void**) &m_pVW);
	m_pGB->QueryInterface(IID_IMediaEvent,	 (void **) &m_pME);
	m_pGB->QueryInterface(IID_IMediaSeeking, (void**) &m_pMS);


	}
	//if the video is not already playing start it this function is only called once at the start of the movie
	if(!m_vVideos[_ID].m_bPlaying)
		m_pGB->RenderFile((LPCWSTR)m_vVideos[_ID].m_szFileName, NULL);
	//set up the window
	m_pVW->put_WindowStyle(dwWindowStyleFlags);
	m_pVW->put_Owner((OAHWND)m_hWnd);

	m_pVW->SetWindowPosition(0,0, _nWindowWidth, _nWindowHeight);
	
	// tell it to notify the message we made 
	m_pME->SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
	// make our window visable
	m_pVW->put_Visible(OATRUE);
	// play the video this doesn't restart the video
		m_pMC->Run();
		m_bPlaying = true;
		m_vVideos[_ID].m_bPlaying = true;
	
		

}

void CVideoMaster::Shutdown()
{
	// shut everything down
	if (m_pME)
	{
		m_pME->Release();
		m_pME = NULL;
	}
	if (m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner(NULL);
		m_pVW->Release();
		m_pVW = NULL;
	}
	if (m_pMC)
	{
		m_pMC->Stop();
		m_pMC->Release();
		m_pMC = NULL;
	}

	

	if (m_pGB)
	{
		m_pGB->Release();
		m_pGB = NULL;
	}

}

void CVideoMaster::HandleEvent()
{
	long evCode, param1, param2;
	HRESULT result;

	while (result = m_pME->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(result))
	{
		result = m_pME->FreeEventParams(evCode, param1, param2);
		// events for if the video is done or the user stopped it
		if ((evCode == EC_COMPLETE) || (evCode == EC_USERABORT))
		{
			Stop();
			//m_bPlaying = false;
			//m_pVW->put_Visible(OAFALSE);
			//Shutdown();
			break;
		}
	}

}
void CVideoMaster::Stop()
{
	REFERENCE_TIME rt = 0;
	// stop the video 
		m_pMC->Stop();
		// get rid of the window
		m_pVW->put_Visible(OAFALSE);
		// reset it so it can be played again
		if(m_pMS)
			m_pMS->SetPositions( &rt,AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
		m_bPlaying = false;
	
}

void CVideoMaster::UnLoad(int _nId)
{
	m_vVideos[_nId].m_szFileName = NULL;
	m_vVideos[_nId].m_nRef = -1;
	m_vVideos[_nId].m_bPlaying = false;
}
void CVideoMaster::RePlay()
{
	REFERENCE_TIME rt = 0;
	// retsets the videos position 
	if(m_pMS)
			m_pMS->SetPositions( &rt,AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	m_bPlaying = true;
	// play it again
	m_pMC->Run();
}
#endif 
