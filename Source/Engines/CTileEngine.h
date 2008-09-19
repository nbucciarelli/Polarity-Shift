////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "CTileEngine.h"
// Author: Leland Nyman (LN)
// Purpose: This file reads in a map and all other necessary components from a file and renders it to the screen.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "windows.h"
#include <string>
using namespace::std;

struct tTile
{
	POINT pos;
};

class CTileEngine
{
private:
	int m_Height;
	int m_Width;
	int m_TileHeight;
	int m_TileWidth;
	int m_nImageID;

	vector<vector<tTile*>> m_vMap;
	vector<RECT> m_vCollision;
	vector<POINT> m_vCubelist;
	vector<POINT> m_vEnemyList;
	vector<POINT> m_vTurretList;
	vector<POINT> m_vSwitchList;
	vector<POINT> m_vDoorList;
	vector<POINT> m_vTriggerList;
	vector<POINT> m_vTrapList;
	vector<RECT> m_vMagnetList;
	vector<POINT> m_vBonusList;
	vector<POINT> m_vInvList;
	POINT m_PlayerSpawn;
	POINT m_PlayerEnd;

	volatile bool ready;
	volatile bool rendering;

	CTileEngine(void);
	~CTileEngine(void);

public:

	static CTileEngine* GetInstance();
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Render"
	//	Last Modified:	August 26, 2008
	//	Purpose:		Renders the map to the screen
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"LoadMap"
	//	Last Modified:	August 26, 2008
	//	Purpose:		Loads in the map and all other components associated with it
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadMap(string szFileName);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Shutdown"
	//	Last Modified:	September 4, 2008
	//	Purpose:		Shuts down the map and all other components associated with it
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void ShutDown();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		Accessors
	//	Last Modified:	September 9, 2008
	//	Purpose:		return specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetImageID() {return m_nImageID;}
	vector<RECT> GetCollisions() {return m_vCollision;}
	vector<POINT> GetCubes() {return m_vCubelist;}
	vector<POINT> GetEnemies() {return m_vEnemyList;}
	vector<POINT> GetTurrets() {return m_vTurretList;}
	vector<POINT> GetSwitches() {return m_vSwitchList;}
	vector<POINT> GetDoors() {return m_vDoorList;}
	vector<POINT> GetTriggers() {return m_vTriggerList;}
	vector<POINT> GetTraps() {return m_vTrapList;}
	POINT GetPlayerSpawn() {return m_PlayerSpawn;}
	POINT GetPlayerEnd() {return m_PlayerEnd;}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		Mutators
	//	Last Modified:	August 26, 2008
	//	Purpose:		modify specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetImageID(int nImageID) {m_nImageID = nImageID;}

};