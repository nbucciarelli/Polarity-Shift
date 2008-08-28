#ifndef CANIMATIONENGINE_H_
#define CANIMATIONENGINE_H_

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
	CAnimationEngine();
	~CAnimationEngine();
	std::string m_Name;
	double m_dFrameTime;
	float m_fFrameTimer;
	double m_dAnimationSpeed;

	void Load(char *szFileName, int nIndex);
	void Play(bool bIsLooping);
	void Stop(void);
	void Resume(void);
	void Update(float fDelta);
	void Render(int nPosX, int nPosY, float fScaleX = 1.0f, float fScaleY = 1.0f, float fRotationX = 0.0f, 
		float fRotationY = 0.0f, float fRotation = 0.0f, unsigned color = 0xFFFFFFFF);

	int GetCurrentFrameIndex(void)		{ return m_nCurrentFrame; }
	int GetNumFrames(void)				{ return m_nNumFrames; }
	tFrame* GetCurrentFrame(void)		{ return &m_pFrames[m_nCurrentFrame]; }
	float GetFrameTimer(void)			{ return m_fFrameTimer; }
	double GetAnimationTimer(void)		{ return m_dAnimationSpeed; }
	bool GetIsPlaying(void)				{ return m_bIsPlaying; }
	bool GetIsLooping(void)				{ return m_bIsLooping; }
	tFrame* GetFrames(void)				{ return m_pFrames; }

	void SetCurrentFrameIndex(int nCurrentFrame)	{ m_nCurrentFrame = nCurrentFrame; }
	void SetNumFrames(int nNumFrames)				{ m_nNumFrames = nNumFrames; }
	void SetFrameTimer(float fFrameTimer)			{ m_fFrameTimer = fFrameTimer; }
	void SetAnimationTimer(double dAnimationTimer)	{ m_dAnimationSpeed = dAnimationTimer; }
	void SetIsPlaying(bool bIsPlaying)				{ m_bIsPlaying = bIsPlaying; }
	void SetIsLooping(bool bIsLooping)				{ m_bIsLooping = bIsLooping; }
	void SetFrames(tFrame* pFrames)					{ m_pFrames = pFrames; }
};

#endif