#include "../Engines/CAnimationEngine.h"
#include "../Wrappers/viewManager.h"
#include "../Engines/CAnimationManager.h"
#include "../Objects/baseObj.h"
#include <fstream>

CAnimationEngine::CAnimationEngine()
{
	m_nCurrentFrame = 0;
	m_nNumFrames = 0;
	m_fFrameTimer = 0.0f;
	m_dAnimationSpeed = 1.0f;
	m_bIsPlaying = true;
	m_bIsLooping = true;
	
	m_pFrames = NULL;
}

CAnimationEngine::~CAnimationEngine()
{
	if (m_pFrames)
		delete[] m_pFrames;

	m_pFrames = NULL;
}

void CAnimationEngine::Load(char *szFileName, int nIndex)
{
	std::ifstream fin;
	fin.open(szFileName, std::ios::binary | std::ios::in);
	char szCharacter = ' ';
	char szCharacter2 = ' ';
	int anacount = 0;
	fin.read((char*)&anacount, sizeof(int));
	for(int i = 0; i < anacount; i++)
	{
		if (i == nIndex)
		{
			char random;
			fin.read(&random, sizeof(char));
			fin.read(&szCharacter, sizeof(char));
			string szName;
			
			while (szCharacter != '\n')
			{
				szName += szCharacter;
				fin.read(&szCharacter, sizeof(char));
			}
		
			//fin.getline((char*)&namez, sizeof(namez.size(), '\n');
			// Animation timer
			double dAnimTemp = 0;
			fin.read((char*)&dAnimTemp, sizeof(double));
			//Frame timer
			double dFrameTemp;
			fin.read((char*)&dFrameTemp, sizeof(double));

			// Number of frames
			fin.read((char*)&m_nNumFrames, sizeof(int));
			// Makes the frames
			m_pFrames = new tFrame[m_nNumFrames];

			for (int i = 0; i < m_nNumFrames; i++)
			{
				// Frame rectangle
				fin.read((char*)&m_pFrames[i].rSource.left, sizeof(int));
				fin.read((char*)&m_pFrames[i].rSource.top, sizeof(int));
				fin.read((char*)&m_pFrames[i].rSource.right, sizeof(int));
				fin.read((char*)&m_pFrames[i].rSource.bottom, sizeof(int));
				
				// Anchor point
				fin.read((char*)&m_pFrames[i].pAnchor.x, sizeof(int));
				fin.read((char*)&m_pFrames[i].pAnchor.y, sizeof(int));

				// Collision rectangle
				fin.read((char*)&m_pFrames[i].rCollision.left, sizeof(int));
				fin.read((char*)&m_pFrames[i].rCollision.top, sizeof(int));
				fin.read((char*)&m_pFrames[i].rCollision.right, sizeof(int));
				fin.read((char*)&m_pFrames[i].rCollision.bottom, sizeof(int));
			}
		}

		char random2;
		fin.read(&random2, sizeof(char));
		fin.read(&szCharacter2, sizeof(char));
		string szName2;

		while (szCharacter2 != '\n')
		{
			szName2 += szCharacter2;
			fin.read(&szCharacter2, sizeof(char));
		}

		//fin.getline((char*)&namez, sizeof(namez.size(), '\n');
		// Animation timer
		double dAnimTemp = 0;
		fin.read((char*)&dAnimTemp, sizeof(double));
		//Frame timer
		double dFrameTime;
		fin.read((char*)&dFrameTime, sizeof(double));

		// Number of frames
		int randnumframes;
		fin.read((char*)&randnumframes, sizeof(int));
		// Makes the frames
		tFrame* prandframes = new tFrame[randnumframes];

		for (int i = 0; i < randnumframes; i++)
		{
			// Frame rectangle
			fin.read((char*)&prandframes[i].rSource.left, sizeof(int));
			fin.read((char*)&prandframes[i].rSource.top, sizeof(int));
			fin.read((char*)&prandframes[i].rSource.right, sizeof(int));
			fin.read((char*)&prandframes[i].rSource.bottom, sizeof(int));

			// Anchor point
			fin.read((char*)&prandframes[i].pAnchor.x, sizeof(int));
			fin.read((char*)&prandframes[i].pAnchor.y, sizeof(int));

			// Collision rectangle
			fin.read((char*)&prandframes[i].rCollision.left, sizeof(int));
			fin.read((char*)&prandframes[i].rCollision.top, sizeof(int));
			fin.read((char*)&prandframes[i].rCollision.right, sizeof(int));
			fin.read((char*)&prandframes[i].rCollision.bottom, sizeof(int));
		}
	}

	fin.close();
	m_bIsLooping = true;
}

void CAnimationEngine::Play(bool bIsLooping)
{
	m_nCurrentFrame = 0;
	m_fFrameTimer = 0.0f;
	m_bIsPlaying = true;
	m_bIsLooping = bIsLooping;
}

void CAnimationEngine::Stop()
{
	m_bIsPlaying = false;
}

void CAnimationEngine::Resume()
{
	m_bIsPlaying = true;
}

void CAnimationEngine::Update(float fDelta)
{
	if(!m_bIsPlaying)
		return;

	m_fFrameTimer += fDelta*(float)m_dAnimationSpeed;
	if ( m_fFrameTimer > m_dFrameTime )
	{
		++m_nCurrentFrame;

		if ( m_bIsLooping && m_nCurrentFrame >= m_nNumFrames )
			m_nCurrentFrame = 0;
		else if ( m_nCurrentFrame >= m_nNumFrames)
		{
			m_nCurrentFrame = 0;
			m_bIsPlaying = false;
		}

		m_fFrameTimer = 0.0f;
	}
}