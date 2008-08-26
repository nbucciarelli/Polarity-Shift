//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CParticleEffect.h"
//	Author:			Nick Bucciarelli
//	Date Created:	July 11, 2008
//	Purpose:		handle particle effects
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
using std::vector;

struct tParticle 
{
	int m_nCurrAColor;
	int m_nCurrRColor;
	int m_nCurrGColor;
	int m_nCurrBColor;
	double m_fAge;
	double m_fXVelocity;
	double m_fYVelocity;
	double m_fCurrScale;
	double m_fCurrXPos;
	double m_fCurrYPos;
	double m_fLifeSpan;	
};


class CParticleEffect
{
private:
	int m_nImageID;

	int m_nNumParticles;
	int m_nStartAlpha;
	int m_nStartRed;
	int m_nStartBlue;
	int m_nStartGreen;

	int m_nEndAlpha;
	int m_nEndRed;
	int m_nEndBlue;
	int m_nEndGreen;

	double m_fXMin;
	double m_fXMax;
	double m_fYMin;
	double m_fYMax;

	double m_fMinLifespan;
	double m_fMaxLifespan;

	double m_fXSpread;
	double m_fYSpread;

	double m_fStartScale;
	double m_fEndScale;

	bool m_bIsActive;
	bool m_bIsLooping;

	vector<tParticle*> vParticles;
	vector<tParticle*> vDeadParticles;
public:
	CParticleEffect(void);
	~CParticleEffect(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Update"
	//	Last Modified:	July 11, 2008
	//	Purpose:		Updates the particle
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Update(float fDelta);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Render"
	//	Last Modified:	July 11, 2008
	//	Purpose:		Renders a particle
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render(int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotX, float fRotY, float fRot, unsigned nColor );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Reset"
	//	Last Modified:	July 11, 2008
	//	Purpose:		Resets the particle
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Reset(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"ResetParticleValues"
	//	Last Modified:	July 14, 2008
	//	Purpose:		Resets the particle
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void ResetParticleValues(tParticle*);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		Accessors
	//	Last Modified:	July 11, 2008
	//	Purpose:		return specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetImageID(void) { return m_nImageID; }
	int GetNumParticles(void) { return m_nNumParticles; }
	int GetStartAlpha(void) { return m_nStartAlpha; }
	int GetStartRed(void) { return m_nStartRed; }
	int GetStartGreen(void) { return m_nStartGreen; }
	int GetStartBlue(void) { return m_nStartBlue; }
	int GetEndAplha(void) { return m_nEndAlpha; }
	int GetEndRed(void) { return m_nEndRed; }
	int GetEndGreen(void) { return m_nEndGreen; }
	int GetEndBlue(void) { return m_nEndBlue; }
	double GetMinXVelocity(void) { return m_fXMin; }
	double GetMaxXVelocity(void) { return m_fXMax; }
	double GetMinYVelocity(void) { return m_fYMin; }
	double GetMaxYVelocity(void) { return m_fYMax; }
	double GetMinLifespan(void) { return m_fMinLifespan; }
	double GetMaxLifespan(void) { return m_fMaxLifespan; }
	double GetXSpread(void) { return m_fXSpread; }
	double GetYSpread(void) { return m_fYSpread; }
	double GetStartScale(void) { return m_fStartScale; }
	double GetEndScale(void) { return m_fEndScale; }
	bool GetIsActive(void) { return m_bIsActive; }
	bool GetIsLooping(void) { return m_bIsLooping; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		Mutators
	//	Last Modified:	July 11, 2008
	//	Purpose:		Mutate specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetImageID(int nImageID) { m_nImageID = nImageID; }
	void SetNumParticles(int nNumParticles) { m_nNumParticles = nNumParticles; }
	void SetStartAlpha(int nStartAlpha) { m_nStartAlpha = nStartAlpha; }
	void SetStartRed(int nStartRed) { m_nStartRed = nStartRed; }
	void SetStartGreen(int nStartGreen) { m_nStartGreen = nStartGreen; }
	void SetStartBlue(int nStartBlue) { m_nStartBlue = nStartBlue; }
	void SetEndAlpha(int nEndAlpha) { m_nEndAlpha = nEndAlpha; }
	void SetEndRed(int nEndRed) { m_nEndRed = nEndRed	; }
	void SetEndGreen(int nEndGreen) { m_nEndGreen = nEndGreen; }
	void SetEndBlue(int nEndBlue) {  m_nEndBlue = nEndBlue; }
	void SetMinXVelocity(double fXMin) {  m_fXMin = fXMin; }
	void SetMaxXVelocity(double fXMax) { m_fXMax = fXMax; }
	void SetMinYVelocity(double fYMin) { m_fYMin = fYMin; }
	void SetMaxYVelocity(double fYMax) { m_fYMax = fYMax; }
	void SetMinLifeSpan(double fMinLifespan) { m_fMinLifespan = fMinLifespan; }
	void SetMaxLifeSpan(double fMaxLifespan) { m_fMaxLifespan = fMaxLifespan; }
	void SetXSpread(double fXSpread) { m_fXSpread = fXSpread; }
	void SetYSpread(double fYSpread) { m_fYSpread = fYSpread; }
	void SetStartScale(double fStartScale) { m_fStartScale = fStartScale; }
	void SetEndScale(double fEndScale) { m_fEndScale = fEndScale; }
	void SetIsActive(bool bIsActive) { m_bIsActive = bIsActive; }
	void SetIsLooping(bool bIsLooping) { m_bIsLooping = bIsLooping; }

};