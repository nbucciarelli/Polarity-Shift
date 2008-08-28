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

	volatile bool ready;

public:
	CTileEngine(void);
	~CTileEngine(void);

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
	//	Function:		Accessors
	//	Last Modified:	August 26, 2008
	//	Purpose:		return specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetImageID() {return m_nImageID;}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		Mutators
	//	Last Modified:	August 26, 2008
	//	Purpose:		modify specified type
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetImageID(int nImageID) {m_nImageID = nImageID;};


};