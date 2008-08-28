//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CParticleEffect.cpp"
//	Author:			Nick Bucciarelli
//	Last Modified:	August 25, 2008
//	Purpose:		handle particle animations
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CParticleEffect.h"
#include "../Wrappers/viewManager.h"
#include "../helpers/physics.h"

#define RAND_FLOAT(min,max) (((rand()/(double)RAND_MAX)*((max)-(min)))+(min))

CParticleEffect::CParticleEffect(void)
{

}
CParticleEffect::~CParticleEffect(void)
{
	for (unsigned i = vParticles.size() ; i >= 0 ; --i)
	{
		delete vParticles[i];
		//vParticles.pop_back();
	}
	
	for (unsigned i = vDeadParticles.size() ; i >= 0 ; --i)
	{
		delete vDeadParticles[i];
		//vDeadParticles.pop_back();
	}
	vParticles.clear();
	vDeadParticles.clear();

}
void CParticleEffect::Update(float fDelta)
{
	if (!m_bIsActive)
		return;
	for(unsigned i = 0 ; i < vParticles.size() ; ++i)
	{
		vParticles[i]->m_fAge -= fDelta;
		vParticles[i]->m_fCurrXPos += vParticles[i]->m_fXVelocity;
		vParticles[i]->m_fCurrYPos += vParticles[i]->m_fYVelocity;
		vParticles[i]->m_nCurrAColor = (int)(((GetMaxLifespan() - vParticles[i]->m_fAge) / GetMaxLifespan()) * (GetEndAplha() - GetStartAlpha()) + GetStartAlpha());
		vParticles[i]->m_nCurrRColor = (int)(((GetMaxLifespan() - vParticles[i]->m_fAge) / GetMaxLifespan()) * (GetEndRed() - GetStartRed()) + GetStartRed()); 
		vParticles[i]->m_nCurrGColor = (int)(((GetMaxLifespan() - vParticles[i]->m_fAge) / GetMaxLifespan()) * (GetEndGreen() - GetStartGreen()) + GetStartGreen());	
		vParticles[i]->m_nCurrBColor = (int)(((GetMaxLifespan() - vParticles[i]->m_fAge) / GetMaxLifespan()) * (GetEndBlue() - GetStartBlue()) + GetStartBlue()); 
		vParticles[i]->m_fCurrScale = ((GetMaxLifespan() - vParticles[i]->m_fAge)/GetMaxLifespan()) * (GetEndScale() - GetStartScale()) + GetStartScale();
		if (vParticles[i]->m_fAge <= 0 && m_bIsLooping)
			ResetParticleValues(vParticles[i]);
	}/*
	 if (m_bIsLooping)
	 for (unsigned i = 0; i < vDeadParticles.size(); ++i)
	 {
	 // update the dead particles, if thier age <= 0 put them into the live vector
	 if (vDeadParticles[i]->m_fAge <= 0)
	 {
	 vParticles.push_back(vDeadParticles[i]);	
	 }
	 }
	 */

	
	// first time it looks like an explosion
	for (unsigned i = 0; i < vDeadParticles.size(); ++i)
	{
		if (!vDeadParticles[i]) continue;
		vDeadParticles[i]->m_fAge -= fDelta;
		if (vDeadParticles[i]->m_fAge <= 0)
		{
			vParticles.push_back(vDeadParticles[i]);
			vDeadParticles[i]=NULL;
		}
	}

}
void CParticleEffect::Render(int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotX, float fRotY, float fRot, unsigned nColor )
{
	matrix transform;

	for (unsigned i = 0 ; i < vParticles.size() ; ++i)
	{
		calc::matrixTransform(transform,
			vector3((float)vParticles[i]->m_fCurrXPos + nPosX, (float)vParticles[i]->m_fCurrYPos + nPosY, 0),
			vector3((float)vParticles[i]->m_fCurrScale, (float)vParticles[i]->m_fCurrScale, 0));

		viewManager::getInstance()->drawTexture(m_nImageID,
			NULL,
			&transform, 0, 0, viewManager::getInstance()->color_argb(vParticles[i]->m_nCurrAColor, vParticles[i]->m_nCurrRColor, vParticles[i]->m_nCurrGColor, vParticles[i]->m_nCurrBColor));
	}
}
void CParticleEffect::Reset(void)
{
	//Assume emitter is 0,0
	/*	int m_nCurrAColor;
	int m_nCurrRColor;
	int m_nCurrGColor;
	int m_nCurrBColor;
	int m_nXVelocity;
	int m_nYVelocity;
	int m_nCurrScale;
	float m_fLifeSpan;*/
	for (int i = 0; i < m_nNumParticles; ++i)
	{
		tParticle *p = new tParticle();
		p->m_nCurrAColor = 	GetStartAlpha();
		p->m_nCurrRColor = 	GetStartRed();
		p->m_nCurrGColor = 	GetStartGreen();
		p->m_nCurrBColor = 	GetStartBlue();
		p->m_fAge = RAND_FLOAT(GetMinLifespan(), GetMaxLifespan());
		p->m_fCurrScale = GetStartScale();
		p->m_fCurrXPos = RAND_FLOAT(0 - (int)(GetXSpread() / 2), (int)(GetXSpread() /2) + 0);
		p->m_fCurrYPos = RAND_FLOAT(0 - (int)(GetYSpread() / 2), (int)(GetYSpread() /2) + 0);
		p->m_fXVelocity = RAND_FLOAT(GetMinXVelocity(), GetMaxXVelocity());
		p->m_fYVelocity = RAND_FLOAT(GetMinYVelocity(), GetMaxYVelocity());
		vParticles.push_back(p);
	}
	
}
void CParticleEffect::ResetParticleValues(tParticle* pParticle)
{
	pParticle->m_nCurrAColor = 	GetStartAlpha();
	pParticle->m_nCurrRColor = 	GetStartRed();
	pParticle->m_nCurrGColor = 	GetStartGreen();
	pParticle->m_nCurrBColor = 	GetStartBlue();
	pParticle->m_fAge = RAND_FLOAT(GetMinLifespan(), GetMaxLifespan());
	pParticle->m_fCurrScale = GetStartScale();
	pParticle->m_fCurrXPos = RAND_FLOAT(0 - (int)(GetXSpread() / 2), (int)(GetXSpread() /2) + 0);
	pParticle->m_fCurrYPos = RAND_FLOAT(0 - (int)(GetYSpread() / 2), (int)(GetYSpread() /2) + 0);
	pParticle->m_fXVelocity = RAND_FLOAT(GetMinXVelocity(), GetMaxXVelocity());
	pParticle->m_fYVelocity = RAND_FLOAT(GetMinYVelocity(), GetMaxYVelocity());

}

void CParticleEffect::ShutDown()
{
	for (unsigned i = vParticles.size() ; i >= 0 ; --i)
	{
		delete vParticles[i];
		//vParticles.pop_back();
	}

	for (unsigned i = vDeadParticles.size() ; i >= 0 ; --i)
	{
		delete vDeadParticles[i];
		//vDeadParticles.pop_back();
	}
	vParticles.clear();
	vDeadParticles.clear();

}
