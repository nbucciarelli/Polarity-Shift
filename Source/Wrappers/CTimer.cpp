////////////////////////////////////////////////////////////////////////////////////////////
// Filename
//	CTimer.cpp
//
// Author(s)
//	David Brown
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Purpose
//	A high resolution timer with stop watch features (i.e. Start, Stop, Reset).  
//
//	Functions just like a stop watch (i.e. Calling Stop(), then Start() again, continues timing
//	from where it left off).
//	
////////////////////////////////////////////////////////////////////////////////////////////

#include "CTimer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::CTimer() - Constructor 
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CTimer::CTimer()
{
	LONGLONG llTicksPerSecond = 0;

	QueryPerformanceFrequency( (LARGE_INTEGER *)&llTicksPerSecond );

	m_dTimeScale = 1.0 / (double)llTicksPerSecond; // Remember the time scale (only 1 division).
	// Saves us further division because we can multiply
	// by this value and get the same result.

	m_llStartTick					= 0;
	m_dTotalElapsedTime	= 0.0;
	m_bIsTiming					= false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::CTimer() - Destructor 
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CTimer::~CTimer()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::Start() - Starts the timer.
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Start()
{
	// If we aren't currently timing, get the start tick
	if (!m_bIsTiming)
	{
		m_bIsTiming = true;
		QueryPerformanceCounter( (LARGE_INTEGER *)&m_llStartTick );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::Stop() - Stops the timer.
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
// Note:
//	Call Start() again after stopping the timer to continue timing from where the timer stopped.
//	(i.e. if the timer was at 2.3 secs when it was stopped, call Start() to continue from 2.3 secs)
///////////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Stop()
{
	m_dTotalElapsedTime = GetTime(); // put the total elapsed time in m_dTotalElapsedTime

	m_bIsTiming	= false; // Stop timing
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::Reset() - Resets the timer.  Sets the total elapsed time to 0.0.
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CTimer::Reset()
{
	m_bIsTiming					= false;
	m_dTotalElapsedTime	= 0.0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Name - Purpose 
//  CTimer::GetTime() - Gets the time elapsed.  If the timer isn't currently running, returns
//						what the total time was when the timer was stopped.
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 15th, 2005
//
// Input  
//	none
//
// Output  
//	none
//
///////////////////////////////////////////////////////////////////////////////////////////////////
double CTimer::GetTime()
{
	double dElapsedTime = 0.0;

	if (m_bIsTiming)
	{
		LONGLONG llCurTick = 0;

		// Get the current Tick
		QueryPerformanceCounter( (LARGE_INTEGER *)&llCurTick );

		// Calculate the time elapsed from start tick to the current time
		dElapsedTime = ((double)llCurTick - (double)m_llStartTick) * m_dTimeScale;

		// Take into account previously elapsed time (i.e. the timer was stopped then started again)
		dElapsedTime += m_dTotalElapsedTime;
	}
	else
		dElapsedTime = m_dTotalElapsedTime;

	return dElapsedTime;
}