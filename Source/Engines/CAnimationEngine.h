#ifndef CANIMATIONENGINE_H_
#define CANIMATIONENGINE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �CAnimationEngine.h�
// Author: Jared Hamby (JH)
// Purpose: This is the header file for the animation engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include "SGD_Math.h"
#include <windows.h>
#include <string>
using std::string;

class viewManager;
class baseObj;

struct tFrame
{
	RECT rSource;
	RECT rCollision;
	POINT pAnchor;

	int GetWidth() { return rSource.right - rSource.left; }
	int GetHeight() { return rSource.bottom - rSource.top; }
};

class CAnimationEngine
{
private:
	int m_nCurrentFrame;
	int m_nNumFrames;
	bool m_bIsPlaying;
	bool m_bIsLooping;

	tFrame* m_pFrames;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Constructor�
	// Last modified: August 27, 2008
	// Purpose: This is the constructor for the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CAnimationEngine();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Deconstructor�
	// Last modified: August 27, 2008
	// Purpose: This is the deconstructor for the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~CAnimationEngine();
	std::string m_Name;
	double m_dFrameTime;
	float m_fFrameTimer;
	double m_dAnimationSpeed;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Load�
	// Last modified: August 27, 2008
	// Purpose: This loads a file for the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Load(char *szFileName, int nIndex);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Play�
	// Last modified: August 27, 2008
	// Purpose: This plays the animation from the engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Play(bool bIsLooping);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Stop�
	// Last modified: August 27, 2008
	// Purpose: This stops the animation
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Stop(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Resume�
	// Last modified: August 27, 2008
	// Purpose: This resumes the animation
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Resume(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Update�
	// Last modified: August 27, 2008
	// Purpose: This is the update for changing frames in the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float fDelta);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Accessors�
	// Last modified: August 27, 2008
	// Purpose: These are the accessors for the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetCurrentFrameIndex(void)		{ return m_nCurrentFrame; }
	int GetNumFrames(void)				{ return m_nNumFrames; }
	tFrame* GetCurrentFrame(void)		{ return &m_pFrames[m_nCurrentFrame]; }
	float GetFrameTimer(void)			{ return m_fFrameTimer; }
	double GetAnimationTimer(void)		{ return m_dAnimationSpeed; }
	bool GetIsPlaying(void)				{ return m_bIsPlaying; }
	bool GetIsLooping(void)				{ return m_bIsLooping; }
	tFrame* GetFrames(void)				{ return m_pFrames; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Mutators�
	// Last modified: August 27, 2008
	// Purpose: These are the mutators for the animation engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetCurrentFrameIndex(int nCurrentFrame)	{ m_nCurrentFrame = nCurrentFrame; }
	void SetNumFrames(int nNumFrames)				{ m_nNumFrames = nNumFrames; }
	void SetFrameTimer(float fFrameTimer)			{ m_fFrameTimer = fFrameTimer; }
	void SetAnimationTimer(double dAnimationTimer)	{ m_dAnimationSpeed = dAnimationTimer; }
	void SetIsPlaying(bool bIsPlaying)				{ m_bIsPlaying = bIsPlaying; }
	void SetIsLooping(bool bIsLooping)				{ m_bIsLooping = bIsLooping; }
	void SetFrames(tFrame* pFrames)					{ m_pFrames = pFrames; }
};

#endif