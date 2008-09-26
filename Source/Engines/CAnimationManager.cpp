////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CAnimationManager.cpp”
// Author: Jared Hamby (JH)
// Purpose: This is the code file for the animation manager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../Engines/CAnimationManager.h"
#include "../Engines/CAnimationEngine.h"
#include "../Helpers/physics.h"
#include "../objects/baseObj.h"
#include <fstream>

CAnimationManager* CAnimationManager::GetInstance(void)
{
	//static CAnimationManager instance;
	return new CAnimationManager;
}

void CAnimationManager::releaseInstance()
{
	references--;

	if(references == 0)
	{
		this->Shutdown();
		delete this;
	}
}

void CAnimationManager::acquireInstance()
{
	references++;
}

#if 1 == 2

void CAnimationManager::Load(const char* szFileName, baseObj* object)
{
	std::ifstream fin;
	fin.open(szFileName, std::ios::binary | std::ios::in);
	if(!szFileName)
		return;
	int anacount;
	fin.read((char*)&anacount, sizeof(int));
	char szCharacter = ' ';
	for(int i = 0; i < anacount; i++)
	{
		CAnimationEngine* newEngine = new CAnimationEngine();
		char random;
		fin.read(&random, sizeof(char));
		//fin.getline((char*)&newEngine->m_Name, sizeof(newEngine->m_Name.size()), '\n');
		char szName = ' ';

		while (szName != '\n')
		{
			newEngine->m_Name += szName;
			fin.read(&szName, sizeof(char));
		}
		// Animation timer
		fin.read((char*)&newEngine->m_dAnimationSpeed, sizeof(double));		
		//Frame timer
		fin.read((char*)&newEngine->m_dFrameTime, sizeof(double));
		m_pAE.push_back(newEngine);
		m_pBase = object;
		newEngine->Load(szFileName, i);
		// Number of frames
		int nrannumframes;
		fin.read((char*)&nrannumframes, sizeof(int));
		// Makes the frames
		tFrame* randomframes = new tFrame[nrannumframes];

		for (int i = 0; i < nrannumframes; i++)
		{
			// Frame rectangle
			fin.read((char*)&randomframes[i].rSource.left, sizeof(int));
			fin.read((char*)&randomframes[i].rSource.top, sizeof(int));
			fin.read((char*)&randomframes[i].rSource.right, sizeof(int));
			fin.read((char*)&randomframes[i].rSource.bottom, sizeof(int));

			// Anchor point
			fin.read((char*)&randomframes[i].pAnchor.x, sizeof(int));
			fin.read((char*)&randomframes[i].pAnchor.y, sizeof(int));

			// Collision rectangle
			fin.read((char*)&randomframes[i].rCollision.left, sizeof(int));
			fin.read((char*)&randomframes[i].rCollision.top, sizeof(int));
			fin.read((char*)&randomframes[i].rCollision.right, sizeof(int));
			fin.read((char*)&randomframes[i].rCollision.bottom, sizeof(int));
		}	

		delete[] randomframes;
	}
}

#else

void CAnimationManager::Load(const char* szFileName, baseObj* object)
{
	//Let me start of by saying that the way this entire class is arranged is
	//pretty convoluded, and not very practical for the idea of a "manager."
	//And I'm not going to spend a couple of days re-writing the whole thing.

	std::ifstream reader;

	reader.open(szFileName, std::ios_base::in | std::ios_base::binary);

	//It's nice to check fail.
	if(reader.fail())
	{
		MessageBox(NULL, "Failed to load Animation", "Error", MB_OK);
		return;
	}

	m_pBase = object;

	//How many "engines" will be collected today.
	int animCount = 0;
	reader.read((char*)&animCount, sizeof(int));

	for(int c = 0; c < animCount; c++)
	{
		CAnimationEngine* engine = new CAnimationEngine();

		//C# strings save out their size before them.
		char strSize = 0;
		reader.read(&strSize, sizeof(char));

		//Collect name.
		char buffer[100] = {0};
		reader.read(buffer, strSize);

		//There's a newline in there for some reason or another.  Grab it & dump.
		reader.read(&strSize, sizeof(char));

		//Tell the engine its title.
		engine->m_Name = buffer;

		//Now collect some variables that serve some purpose.

		// Animation timer
		reader.read((char*)&engine->m_dAnimationSpeed, sizeof(double));		
		//Frame timer
		reader.read((char*)&engine->m_dFrameTime, sizeof(double));

		//Now we can get the frames.

		int frameCount = 0;
		reader.read((char*)&frameCount, sizeof(int));
		tFrame *frames = new tFrame[frameCount];
		engine->SetNumFrames(frameCount);

		for (int i = 0; i < frameCount; i++)
		{
			// Frame rectangle
			reader.read((char*)&frames[i].rSource.left, sizeof(int));
			reader.read((char*)&frames[i].rSource.top, sizeof(int));
			reader.read((char*)&frames[i].rSource.right, sizeof(int));
			reader.read((char*)&frames[i].rSource.bottom, sizeof(int));

			// Anchor point
			reader.read((char*)&frames[i].pAnchor.x, sizeof(int));
			reader.read((char*)&frames[i].pAnchor.y, sizeof(int));

			// Collision rectangle
			reader.read((char*)&frames[i].rCollision.left, sizeof(int));
			reader.read((char*)&frames[i].rCollision.top, sizeof(int));
			reader.read((char*)&frames[i].rCollision.right, sizeof(int));
			reader.read((char*)&frames[i].rCollision.bottom, sizeof(int));
		}

		engine->SetFrames(frames);

		//And now that we have those, we can add this thing to The List.
		m_pAE.push_back(engine);

	}

}

#endif

void CAnimationManager::Shutdown(void)
{
	/*for (int i = (int)m_pAE.size()-1; i > 0; --i)
	{
		delete m_pAE[i];
		m_pAE.pop_back();
	}

	m_pAE.clear();*/

	while(m_pAE.size())
	{
		delete m_pAE.back();
		m_pAE.pop_back();
	}
}

/*void CAnimationManager::Render(int ID, int nPosX, int nPosY, float fScaleX, float fScaleY, float fRotationX, float fRotationY, float fRotation, unsigned int color)
{
	m_pTM->drawTexture(m_pBase->getImgId(),
		&vector3((float)(nPosX - (m_pAE[ID]->GetCurrentFrame()->pAnchor.x - m_pAE[ID]->GetCurrentFrame()->rSource.left)),
			(float)(nPosY - (m_pAE[ID]->GetCurrentFrame()->pAnchor.y - m_pAE[ID]->GetCurrentFrame()->rSource.top)),
			0),
		NULL,
		(rect*)(&m_pAE[ID]->GetCurrentFrame()->rSource),
		&vector3((float)m_pAE[ID]->GetCurrentFrame()->pAnchor.x,
			(float)m_pAE[ID]->GetCurrentFrame()->pAnchor.y, 0),
		color);
}*/

void CAnimationManager::Render(int ID, matrix* transform)
{
	m_pTM->drawTexture(m_pBase->getImgId(), NULL, transform,
		(rect*)(&m_pAE[ID]->GetCurrentFrame()->rSource));
}

void CAnimationManager::Update(float fDelta)
{
	for(unsigned int i = 0; i < m_pAE.size(); i++)
	{
		if (m_pAE[i]->GetIsPlaying() == true)
			m_pAE[i]->Update(fDelta);
	}
}

CAnimationEngine* CAnimationManager::GetEngine(unsigned int nIndex)
{
	if(nIndex > m_pAE.size() || nIndex < 0)
		return NULL;

	return m_pAE[nIndex];
}