//______________________________________________________________________________
//
// File:...... CSGD_FModManager.cpp
//
// Author:.... Robert Martinez (Bobby)
//
// Date:...... 12/29/2007 12:16:36 AM
//
// Purpose:.... function definitions for fMod wrapper class
//______________________________________________________________________________

#include "CSGD_FModManager.h"
#include <assert.h>
#include "..\game.h"

CSGD_FModManager CSGD_FModManager::m_pInstance;	//	initialization of single class instance 

CSGD_FModManager::CSGD_FModManager(void) : m_pSystem(NULL) , m_hWnd(0), m_dwCurrentTime(0), m_dwPreviousTime(0), m_dTimeAccumulator(0) 
{
}

CSGD_FModManager::~CSGD_FModManager(void) 
{
}

CSGD_FModManager *CSGD_FModManager::GetInstance( void )
{
	return &m_pInstance;	//	return instance of the class
}

bool CSGD_FModManager::InitFModManager( HWND hWnd, int nMaxChannels, FMOD_INITFLAGS  unFlags, void *vExtraDriverData ) 
{
	//	Paranoid error check
	if (!hWnd || nMaxChannels < 0 || unFlags < 0) return false;

	//	get handle to window 
	m_hWnd = hWnd;

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;

	//	Create the main system object.
	if ((result = FMOD::System_Create(&m_pSystem)) != FMOD_OK)
	{
		FMODERR( result );
	}

	// Initialize FMOD.
	if ((result = m_pSystem->init( nMaxChannels, unFlags, vExtraDriverData ) ) != FMOD_OK)
	{
		FMODERR( result );
	}

	//	init timers from garbage collection
	m_dwCurrentTime = GetTickCount();
	m_dwPreviousTime = GetTickCount();

	return true;
}

int CSGD_FModManager::LoadSound(const char *szFilename, FMOD_MODE unMode ) 
{
	if( !m_pSystem ) return -1;

	if( !szFilename ) return -1;

	int nHandle = -1;	//	an interger that is returned to the user so they can reference this sound again
	int counter = 0;	//	used to keep track of where in a loop we we're indexing in

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;

	vector<tSoundInfo>::iterator vIter = m_SoundList.begin();	

	while( vIter != m_SoundList.end() ) 
	{
		if( _stricmp( (*vIter).filename, szFilename ) == 0 ) 
		{
			(*vIter).ref++;
			nHandle = counter;
			break;
			//	only hole in this logic:
			//	what if a sound is loaded that has a different FMOD_MODE?
			//	has to be taken into account
		}
		vIter++;
		counter++;
	}

	if( nHandle != -1) return nHandle;		//	We've found our handle!

	//	re-initing indexer
	counter = 0;


	//	if one of the already used sounds is done with, reuse that spot
	//	NOTE:
	//	(only use this if I can reset each object
	while( vIter != m_SoundList.end() ) 
	{
		if( (*vIter).ref == 0 ) 
		{
			nHandle = counter;
			break;
		}
		vIter++;
		counter++;
	}

	//	the sound has not been loaded before,
	//	so a new one must be made based on this file
	tSoundInfo newSound;

	if( nHandle == -1 ) 
	{
		//	copy name into object
		strncpy_s( newSound.filename, _MAX_PATH, szFilename, strlen( szFilename ) );

		//	first instance of this sound
		newSound.ref = 1;

		//	set flags to the sound
		newSound.unSoundFormat |= unMode;

		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE
		if( ( result = m_pSystem->createSound( newSound.filename, newSound.unSoundFormat, 0, &newSound.fmSound ) ) != FMOD_OK )
		{
			FMODERR( result );
		}

		//	push new allocated sound onto vector
		m_SoundList.push_back( newSound );

		return (int)m_SoundList.size() - 1;

	}
	else
	{
		//	release anything still in m_SoundList[ nHandle ];
		//	then copy above 

	}

	return nHandle;
}

bool CSGD_FModManager::UnloadSound( int nID )
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	if( m_SoundList[nID].ref == 0 || ( result = m_SoundList[nID].fmSound->release() ) == FMOD_OK ) 
	{
		ZeroMemory(m_SoundList[nID].filename, _MAX_PATH);
		m_SoundList[nID].fmSound = NULL;
		m_SoundList[nID].ref = 0;
		bOutcome = true;
	}
	else
		FMODERR( result );

	return bOutcome;
}

bool CSGD_FModManager::PlaySound( int nID, bool bIsMusic) 
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	Address of a channel handle pointer that receives the newly playing channel
	FMOD::Channel *channel;
	//	used to catch fMod-specific error codes
	FMOD_RESULT result;

	if( ( result = m_pSystem->playSound(FMOD_CHANNEL_FREE, m_SoundList[ nID ].fmSound , false, &channel) ) != FMOD_OK ) 
	{
		FMODERR( result );
	}

	//	hold on to channel pointer for late use
	m_SoundList[ nID ].m_SoundChannels.push_back( channel );

	if (bIsMusic)
		SetVolume(nID, (float)(game::GetInstance()->GetMusicLevel()/100.0f));
	else
		SetVolume(nID, (float)(game::GetInstance()->GetSFXLevel()/100.0f));

	//	return success
	return true;
}

bool CSGD_FModManager::ResetSound( int nID )
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() ) 
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			(*iter)->setPosition( 0,  FMOD_TIMEUNIT_MS);
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::StopSound( int nID )
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() )
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			(*iter)->stop( );
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::SetVolume( int nID, float fVolume )
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() ) 
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			(*iter)->setVolume( fVolume );
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::SetFrequency( int nID, float fFreq )
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() )
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			(*iter)->setFrequency( fFreq );
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::SetPan( int nID, float fPan ) 
{
	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() )
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			(*iter)->setPan( fPan );
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::IsSoundPlaying( int nID ) {

	if( !m_pSystem ) return false;

	//	check that ID is in range
	assert( nID > -1 && nID < (int)m_SoundList.size() && "ID is out of range" );

	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;

	//	iterate all the channels in this sound
	list<FMOD::Channel*>::iterator iter = m_SoundList[nID].m_SoundChannels.begin();

	while( iter != m_SoundList[nID].m_SoundChannels.end() ) 
	{
		if( ( result = (*iter)->isPlaying( &bOutcome ) ) == FMOD_OK ) 
		{
			break;
		}
		iter++;
	}

	//	return the outcome of this function
	return bOutcome;
}

bool CSGD_FModManager::Update( void ) 
{
	//	used to catch fMod-specific error codes
	FMOD_RESULT result;

	if( ( result = m_pSystem->update() ) != FMOD_OK)
	{
		FMODERR( result );
	}

	//	TODO: Add garbage collector here

	m_dwPreviousTime = m_dwCurrentTime;

	m_dwCurrentTime = GetTickCount();

	//	how much time has past since our last call to update
	double dDeltaTime = static_cast<double>( (m_dwCurrentTime - m_dwPreviousTime) / 1000.0f);

	m_dTimeAccumulator += dDeltaTime;

	//	collect garbage fMod handles every few seconds
	if( m_dTimeAccumulator >= TIMER_COUNT ) 
	{
   		ReduceHandleCount( );
		//	reset accumulation buffer
		m_dTimeAccumulator -= TIMER_COUNT;
	}

	//	Success!
	return true;
}

void CSGD_FModManager::ReduceHandleCount( void ) 
{
	//	used to catch fMod-specific error codes
	FMOD_RESULT result;
	bool bOutcome = false;
	vector<tSoundInfo>::iterator vecIter = m_SoundList.begin();

	//	for all sound file loaded, check to see on their channel handles
 	while( vecIter != m_SoundList.end() )
	{
		list<FMOD::Channel *>::iterator listIter = (*vecIter).m_SoundChannels.begin();

		while( listIter != (*vecIter).m_SoundChannels.end() ) 
		{

			bOutcome = false;
			//	if the Handle is invalid, get rid of it
			if( ( result = (*listIter)->isPlaying( &bOutcome ) ) == FMOD_ERR_INVALID_HANDLE ) 
			{
				listIter = (*vecIter).m_SoundChannels.erase( listIter );
			}
			else	//	otherwise, keep moving along the list
				listIter++;

		}	//	End of m_SoundChannels loop

		vecIter++;	//	move to next list of handles

	}	//	End of m_SoundList loop 
}

bool CSGD_FModManager::ShutdownFModManager( void ) 
{
	//	iterate through all sounds
	vector<tSoundInfo>::iterator vIter = m_SoundList.begin();
	//	keep trak of where we are in the vector
	int counter = 0;

	for( counter = 0; counter < (int)m_SoundList.size(); counter++) 
	{
		//	go through vector, releasing all sounds
		if( UnloadSound( counter ) ) { }
	}

	//	clear out vector
	m_SoundList.clear();

	//	all done
	return true;
}

