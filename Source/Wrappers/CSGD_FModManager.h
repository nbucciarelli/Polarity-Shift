//______________________________________________________________________________
//
// File:...... CSGD_FModManager.h
//
// Author:.... Robert Martinez (Bobby)
//
// Date:...... 12/29/2007 12:16:25 AM
//
// Purpose:.... fMod management class that wraps up core functionality for the fMod API
//______________________________________________________________________________

#pragma once

#include <windows.h>
#include <vector>
#include <list>
using std::vector;
using std::list;
#include "fmod.hpp"
#include "fmod_errors.h"

#if _DEBUG
	#pragma comment( lib, "fmodexL_vc.lib" ) 
#else
	#pragma comment( lib, "fmodex_vc.lib" ) 
#endif

//	Flag for Loading a Sound and Having it loop.
#define SGD_FMOD_LOOPING	FMOD_LOOP_NORMAL | FMOD_DEFAULT

//	MACRO used for debugging
#ifndef FMODERR
#define FMODERR(errcode) { MessageBox(m_hWnd, FMOD_ErrorString(errcode), "CFMod Error", MB_OK | MB_ICONEXCLAMATION); return false; }
#endif

//	Macro to adjust how often Garbage(Handles) are collected
#define TIMER_COUNT 5.0f

class CSGD_FModManager
{
	FMOD::System *m_pSystem;			//	Private instance of the FMod 'System' API

	//	Struct contains data on the loaded sound files
	struct tSoundInfo 
	{
		char filename[_MAX_PATH];		//	The filename of this wave.
		int	 ref;						//	The number of times this wave has be loaded.
		FMOD_MODE unSoundFormat;		//	Sound description bitfields	
		FMOD::Sound *fmSound;			//	Pointer to FMOD object

		list<FMOD::Channel*> m_SoundChannels;		//	channel object pointers that have been used for this sound 

		//	Default constructor.
		tSoundInfo() 
		{ 
			filename[0] = '\0';
			ref = 0;
			memset(&unSoundFormat, 0, sizeof(unSoundFormat)); 
			fmSound = NULL; 
		}
	};

	vector<tSoundInfo> m_SoundList;					//	List of loaded sound files.

	double m_dTimeAccumulator;						//	buffer used to accumulate the time that has passed
	DWORD m_dwCurrentTime, m_dwPreviousTime;		//	used to find out how much has passed since the object has been updated


	//	Our own handle to the Main Window. Used for debuging, and MessageBox creation
	HWND m_hWnd;

	static CSGD_FModManager m_pInstance;				//	Single priavte Instance of this class 

	//	Default constructor
	CSGD_FModManager( void );
	//	Copy Construtor
	CSGD_FModManager( const CSGD_FModManager & );
	//	Assignment operator
	CSGD_FModManager &operator =( const CSGD_FModManager & );

	//	Destructor
	virtual ~CSGD_FModManager( void );

public:


	////////////////////////////////////////////////////////////////////////
	// Function:  CFModManager *GetInstance()
	// Last Modified : [12/28/2007 11:45:24 PM]
	// 
	// Purpose: Only allow user a single instance of this class,
	//					but can be obtained anywhere in the codebase
	// 
	// Function parameters : 
	//   [] -
	// 
	// Return - [CFModManager *]: single instance of this class
	////////////////////////////////////////////////////////////////////////
	static CSGD_FModManager *GetInstance( void );

	////////////////////////////////////////////////////////////////////////
	// Function:  InitFModManager()
	// Last Modified : [12/31/2007 11:20:32 AM]
	// 
	// Purpose: Create and initialize the fMod System API pointer. Call after GetInstance, but before any other CFModManager function 
	// 
	// Function parameters : 
	//   [hWnd] -					Handle to a window, used for debuging, and MessageBox creation.
	//   [nMaxChannels] -		Maximum number of channels to allot for fMod. Keep this number high.
	//   [unFlags] -				Initialization flags.  This can be a selection of flags bitwise OR'ed together to change the behaviour of FMOD at initialization time.
	//   [vExtraDriverData] -	Driver specific data that can be passed to the output plugin. Optional. Specify 0 or NULL to ignore.
	// 
	// Return - [bool]: true for successful creation of the system object,
	//						  false if not.
	////////////////////////////////////////////////////////////////////////
	bool InitFModManager(HWND hWnd, int nMaxChannels = 50, FMOD_INITFLAGS unFlags = FMOD_INIT_NORMAL, void *vExtraDriverData = NULL);

	////////////////////////////////////////////////////////////////////////
	// Function:  LoadSound()
	// Last Modified : [1/2/2008 10:49:38 AM]
	// 
	// Purpose: Load a specified sound into memory, and hand back a ID to that sound so it can be played later
	// 
	// Function parameters : 
	//   [szFilename] -			The name of a sound file to be loaded into the Manager 
	//   [unMode] -				Sound description bitfields, bitwise OR them together for loading and describing sounds.
	// 
	// Return - [int]: Unique ID for a newly loaded sound, or the ID of that sound if it's been previously loaded
	//
	//				NOTE: if you are returned -1, a failure has occured while loading a sound!
	////////////////////////////////////////////////////////////////////////
	int LoadSound(const char *szFilename, FMOD_MODE  unMode = FMOD_DEFAULT);

	////////////////////////////////////////////////////////////////////////
	// Function:  PlaySound()
	// Last Modified : [1/3/2008 8:02:39 PM]
	// 
	// Purpose: Play a sound that has been loaded into the manager
	// 
	// Function parameters : 
	//   [nID] -							The ID of the sound to play
	// 
	// Return - [bool]: true if sound was played
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool PlaySound(int nID, bool bIsMusic = false);

	////////////////////////////////////////////////////////////////////////
	// Function:  UnloadSound()
	// Last Modified : [1/4/2008 10:15:54 AM]
	// 
	// Purpose: Release a specific sound based on it's ID
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound to unload
	// 
	// Return - [bool]: true if sound was unloaded
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool UnloadSound(int nID);

	////////////////////////////////////////////////////////////////////////
	// Function:  ResetSound()
	// Last Modified : [1/4/2008 10:16:02 AM]
	// 
	// Purpose: Reset sound to it's begining
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound to reset
	// 
	// Return - [bool]: true if sound was reset
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool ResetSound(int nID);

	////////////////////////////////////////////////////////////////////////
	// Function:  StopSound()
	// Last Modified : [1/4/2008 10:16:12 AM]
	// 
	// Purpose: Stop sound from playing in a channel
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound to stop
	// 
	// Return - [bool]: true if sound was stopped
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool StopSound(int nID);

	////////////////////////////////////////////////////////////////////////
	// Function:  SetVolume()
	// Last Modified : [1/4/2008 10:16:30 AM]
	// 
	// Purpose: Set the volume of a specific sound
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound whos volume needs to be changed
	//   [fVolume] -				A linear volume level, from 0.0 to 1.0 inclusive. 0.0 = silent, 1.0 = full volume. Default = 1.0. 
	// 
	// Return - [bool]: true if sound's volume was changed
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////	
	bool SetVolume(int nID, float fVolume);

	////////////////////////////////////////////////////////////////////////
	// Function:  SetFrequency()
	// Last Modified : [1/4/2008 10:16:22 AM]
	// 
	// Purpose: Sets the sound's frequency or playback rate, in HZ. 
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound whos frequency needs to be changed
	//   [fFreq] -					A frequency value in HZ.
	// 
	// Return - [bool]: true if sound's frequency was changed
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool SetFrequency(int nID, float fFreq);

	////////////////////////////////////////////////////////////////////////
	// Function:  SetPan()
	// Last Modified : [1/4/2008 10:17:06 AM]
	// 
	// Purpose: Sets a sounds pan position.  
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound whos pan position needs to be changed
	//   [fPan] -						A left/right pan level, from -1.0 to 1.0 inclusive. -1.0 = Full left, 0.0 = center, 1.0 = full right. Default = 0.0. 
	// 
	// Return - [bool]: true if sound's pan position was changed
	//						 false if there was an error
	////////////////////////////////////////////////////////////////////////
	bool SetPan(int nID, float fPan);

	////////////////////////////////////////////////////////////////////////
	// Function:  IsSoundPlaying()
	// Last Modified : [1/4/2008 10:16:56 AM]
	// 
	// Purpose: Find out whether a spcific sound is playing, or not
	// 
	// Function parameters : 
	//   [nID] -						The ID of the sound that is playing or not
	// 
	// Return - [bool]: true if sound is playing
	//						 false if it is not 
	////////////////////////////////////////////////////////////////////////
	bool IsSoundPlaying(int nID);

	////////////////////////////////////////////////////////////////////////
	// Function:  Update()
	// Last Modified : [1/3/2008 8:02:54 PM]
	// 
	// Purpose: Updates the FMOD system. This should be called once per 'game' tick, or once per frame in your application.
	// 
	// Function parameters : 
	//   [] -
	// 
	// Return - [bool]: true if the fMod System was seccessfully updated
	//						 false there is an error with the fMod System
	////////////////////////////////////////////////////////////////////////
	bool Update(void);

	////////////////////////////////////////////////////////////////////////
	// Function:  ShutdownFModManager()
	// Last Modified : [1/3/2008 8:03:07 PM]
	// 
	// Purpose: Iterate through container of sounds, releasing them using the UnloadSound( ) function
	// 
	// Function parameters : 
	//   [] -
	// 
	// Return - [bool]: true if we've released all sound seccessfully
	////////////////////////////////////////////////////////////////////////
	bool ShutdownFModManager(void);

protected:
	////////////////////////////////////////////////////////////////////////
	// Function:  ReduceHandleCount()
	// Last Modified : [1/10/2008 2:26:50 PM]
	// 
	// Purpose: Reduce the number of invlaid channel handles used to play sounds
	// 
	// Function parameters : 
	//   [] -
	// 
	// Return - [void]: 
	////////////////////////////////////////////////////////////////////////
	void ReduceHandleCount(void);
};
