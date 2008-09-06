
////////////////////////////////////////////////////////////////////////////////////////////
// Filename
//	CTimer.h
//
// Author(s)
//	David Brown
//
// Date of creation
//	July 15th, 2005
//
// Date last updated
//	July 20th, 2005
//
// Purpose
//	A high resolution timer with stop watch features (i.e. Start, Stop, Reset).  
//
//	Functions just like a stop watch (i.e. Calling Stop(), then Start() again, continues timing
//	from where it left off).
//	
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CTIMER_H_
#define __CTIMER_H_

#include <windows.h>

#pragma comment(lib, "winmm.lib")

class CTimer
{
public:

	CTimer();	// Constructor
	virtual ~CTimer();	// Destructor

	void Start();
	void Stop();
	void Reset();
	double GetTime();

private:

	double				m_dTimeScale;					// 1/ticksPerSecond.  For converting ticks into seconds.

	LONGLONG	m_llStartTick;						// The tick when the timer was started

	bool					m_bIsTiming;						// Is it currently timing?
	double				m_dTotalElapsedTime;	// total elapsed time in seconds

};
#endif