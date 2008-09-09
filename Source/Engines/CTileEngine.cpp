////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "CTileEngine.cpp"
// Author: Leland Nyman (LN)
// Purpose: This file reads in a map and all other necessary components from a file and renders it to the screen.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTileEngine.h"
#include "..\Wrappers\viewManager.h"
#include <fstream>

CTileEngine::CTileEngine() : ready(false), rendering(false)
{
	m_Height = 0;
	m_Width = 0;
	m_TileHeight = 0;
	m_TileWidth = 0;
	m_nImageID = -1;

}

void CTileEngine::LoadMap(string szFileName)
{
	ready = false;

	std::ifstream fin;
	fin.open(szFileName.c_str(), std::ios::binary | std::ios::in);

	if(fin.fail())
	{
		MessageBox(NULL, "OHSHI-", "error", MB_OK);
		return;
	}
	//load in tileset file name
	char length;
	fin.read((char*)&length, sizeof(char));
	char burr[255] = {0}; 
	char burf[255] = {0};
	fin.read(burr,length);
	sprintf_s(burf, "Resource/%s", burr);
	SetImageID(viewManager::getInstance()->loadTexture(burf));
	int List;
	fin.read((char*)&List, sizeof(int));
	
	//load in layers
	for (int i = 0; i< List; i++)
	{
		vector<tTile*> layer;
		fin.read((char*)&m_Width, sizeof(int));
		fin.read((char*)&m_Height, sizeof(int));
		fin.read((char*)&m_TileWidth, sizeof(int));
		fin.read((char*)&m_TileHeight, sizeof(int));
		for(int j = 0; j < m_Width*m_Height; j++)
		{
			tTile* tempTile = new tTile;
			fin.read((char*)&tempTile->pos.x, sizeof(int));
			fin.read((char*)&tempTile->pos.y, sizeof(int));

			layer.push_back(tempTile);
		}
		m_vMap.push_back(layer);
	}

	fin.read((char*)&List, sizeof(int));

	//load in collision rects
	for (int i = 0; i < List; i++)
	{
		RECT tempRect;
		int nWidth;
		int nHeight;
		fin.read((char*)&tempRect.left, sizeof(int));
		fin.read((char*)&tempRect.top,sizeof(int));
		fin.read((char*)&nWidth,sizeof(int));
		fin.read((char*)&nHeight,sizeof(int));
		tempRect.right = tempRect.left + nWidth;
		tempRect.bottom = tempRect.top + nHeight;

		m_vCollision.push_back(tempRect);
	}

	fin.read((char*)&List, sizeof(int));
	
	//load in cubes
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vCubelist.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in enemis
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vEnemyList.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in turrets
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vTurretList.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in switches
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vSwitchList.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in switches
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vDoorList.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in switches
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vTriggerList.push_back(temp);
	}

	fin.read((char*)&List, sizeof(int));

	//load in switches
	for(int i = 0; i < List; i ++)
	{
		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		
		m_vTrapList.push_back(temp);
	}

		fin.read((char*)&List, sizeof(int));


		POINT temp;
		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		m_PlayerSpawn = temp;


		fin.read((char*)&temp.x, sizeof(int));
		fin.read((char*)&temp.y, sizeof(int));
		m_PlayerEnd = temp;
		


	fin.close();

	ready = true;

}
void CTileEngine::Render()
{
	if(!ready)
		return;

	RECT source;
	//loop through layers
	rendering = true;
	for(unsigned int h = 0; h < m_vMap.size(); h++)
	{
		for(int i = 0; i < m_Height ; i++)
		{
			for(int j = 0; j < m_Width ; j++)
			{
				source.left = m_vMap[h][i+(j*m_Height)]->pos.x;
				source.right = source.left + m_TileWidth;
				source.top = m_vMap[h][i+(j*m_Height)]->pos.y;
				source.bottom = source.top + m_TileHeight;

				viewManager::getInstance()->drawTexture(GetImageID(),
					&vector3((float)(j* m_TileHeight), (float)(i * m_TileWidth), (float)((-1*h)+1)), 0, (rect*)&source);
			}
		}
	}
	rendering = false;
}

void CTileEngine::ShutDown()
{
	ready = false;
	while(rendering)
	{
		Sleep(0);
	}
	while(m_vMap.size() != 0)
	{
		while(m_vMap.back().size() !=0)
		{
			delete m_vMap.back().back();
			m_vMap.back().pop_back();
		}
		m_vMap.pop_back();
	}
	m_vCollision.clear();
	m_vCubelist.clear();
	m_vEnemyList.clear();
	m_vTurretList.clear();
	m_vSwitchList.clear();
}

CTileEngine::~CTileEngine()
{
	CTileEngine::ShutDown();
}