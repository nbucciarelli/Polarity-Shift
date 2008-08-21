//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"CParticleEffectManager.cpp"
//	Author:			Nick Bucciarelli
//	Date Created:	July 11, 2008
//	Purpose:		handles the particles
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CParticleEffectManager.h"
#include <fstream>
#include <string>

#include "..\Wrappers\viewManager.h"


void CParticleEffectManager::Update(float fDelta)
{
	m_fUpdateTimer -= fDelta;
	if (m_fUpdateTimer > 0.0f)
		return;
	else
		m_fUpdateTimer = .016f;

	for (unsigned i = 0; i < vParticleEffect.size(); ++i)
		vParticleEffect[i]->Update(fDelta);
}


int CParticleEffectManager::LoadEffect(char* szFileName)
{
	CParticleEffect* p = new CParticleEffect;
	std::string szTexture;
	int nData;
	float fData;
	char szCharacter;
	
	std::ifstream fin;
	fin.open(szFileName, std::ios::binary | std::ios::in);
	// Texture
	fin.read(&szCharacter, sizeof(char));
	while (szCharacter != '\0')
	{
		szTexture += szCharacter;
		fin.read(&szCharacter, sizeof(char));
	}
	p->SetImageID(viewManager::getInstance()->loadTexture(szTexture.c_str()));

	// num particles
	fin.read((char*)&nData, sizeof(int));
	p->SetNumParticles(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetStartAlpha(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetStartRed(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetStartGreen(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetStartBlue(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetEndAlpha(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetEndRed(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetEndGreen(nData);
	fin.read((char*)&nData, sizeof(int));
	p->SetEndBlue(nData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMinXVelocity(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMaxXVelocity(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMinYVelocity(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMaxYVelocity(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetStartScale(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetEndScale(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMinLifeSpan(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetMaxLifeSpan(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetXSpread(fData);
	fin.read((char*)&fData, sizeof(float));
	p->SetYSpread(fData);
	p->SetIsActive(false);
	int index = -1;
	for (unsigned i = 0; i < vParticleEffect.size() ; ++i)
	{
		if (vParticleEffect[i] == NULL)
		{
			index = 1;
			break;

		}
	}
	if (index == -1)
	{
		index = (int)vParticleEffect.size();
		vParticleEffect.push_back(p);
	}
	else
		vParticleEffect[index] = p;
	//check size -1

	p->Reset();

	//return the index that the particle is in
	return index;
}
void CParticleEffectManager::Render(int nImageID, int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotX, float fRotY, float fRot, unsigned nColor)
{
	vParticleEffect[nImageID]->Render(nPosX, nPosY, fScaleX, fScaleY, fRotX, fRotY, fRot, nColor);
	
}
void CParticleEffectManager::Play(int nImageID, bool bIsLooping)
{
	vParticleEffect[nImageID]->SetIsActive(true);
	vParticleEffect[nImageID]->SetIsLooping(bIsLooping);
}
void CParticleEffectManager::Stop(int nImageID)
{
	vParticleEffect[nImageID]->SetIsActive(false);
}
void CParticleEffectManager::Unload(int nImageID)
{
	if (vParticleEffect[nImageID]) delete vParticleEffect[nImageID]; vParticleEffect[nImageID] = NULL;
}