#include "objFileLoader.h"
#include <windows.h>

#include <fstream>
using std::ifstream;
#include <exception>
using std::exception;

#include "../Objects/objFactory.h"
#include "../Objects/objManager.h"
#include "../Wrappers/viewManager.h"
#include "../EventSystem/EventManager.h"
#include "../EventSystem/EventIDs.h"
#include "datatypes.h"

#include "../Objects/playerObj.h"
#include "../Objects/enemyObj.h"

#include "../Engines/canimationmanager.h"

enum objectTypes { 
	PLAYER,
	ENEMY,
	SWITCH,
	EXIT,
	PLATFORM,
	CRATE
};

objFileLoader::objFileLoader()
{
	OF = objFactory::getInstance();
}

objFileLoader::~objFileLoader()
{
}

char* objFileLoader::loadObject(char* filename)
{
	ifstream reader;

	reader.open(filename);

	if(reader.fail())
	{
		MessageBox(NULL, "Error Loading Game Object.", "Error", MB_OK);
		return 0;
	}

	int holder = 0;
	short shorty = 0;
	short type = 0;

	//Grab & ignore version information
	reader.read((char*)&holder, sizeof(int));

	//Grab object class
	reader.read((char*)&type, sizeof(short));

	// *((short*)&holder);

	//A pointer to do stuff.
	baseObj* obj = NULL;

	switch(type)
	{
	case PLAYER:
		obj = new playerObj;
		break;
	case ENEMY:
		obj = new enemyObj;
		break;
	case SWITCH:
	case EXIT:
	case PLATFORM:
		return 0;
	case CRATE:
		obj = new movingObj;
		break;
	default:
		return 0;
	}

	char len = 0;
	//Grab name length.
	reader.read(&len, sizeof(char));

	char textbuffer[100] = {0};

	//Read name, assign as factory designator
	reader.read(textbuffer, len);
	IDType objectID = textbuffer;

	memset(textbuffer, 0, sizeof(textbuffer));

	//grab anim path.
	reader.read(&len, sizeof(char));
	reader.read(textbuffer, len);

	char animFile[100] = {0};

	if(textbuffer[0] == '\\')
		strcpy_s(animFile, len, &textbuffer[1]);
	else
		strcpy_s(animFile, len, textbuffer);

	memset(textbuffer, 0, sizeof(textbuffer));

	//grab texture path length
	reader.read(&len, sizeof(char));

	reader.read(textbuffer, len);

	char textureFile[100] = {0};

	if(textbuffer[0] == '\\')
		strcpy_s(textureFile, len, &textbuffer[1]);
	else
		strcpy_s(textureFile, len, textbuffer);

	//Read center information.  (Dump for now.)
	objectPoint center;
	reader.read((char*)&shorty, sizeof(short));
	center.coords.x = (float)shorty;
	reader.read((char*)&shorty, sizeof(short));
	center.coords.y = (float)shorty;
	reader.read((char*)&shorty, sizeof(short));
	center.mass = (float)shorty;

	//Now grab point count
	reader.read((char*)&holder, sizeof(short));

	polygon * poly = new polygon;

	short vc = *((short*)&holder);
	short ptDat = 0;

	poly->vertecies = new objectPoint[vc];
	poly->vertexCount = vc;

	for(int c = 0; c < vc; c++)
	{
		reader.read((char*)&ptDat, sizeof(short));
		poly->vertecies[c].coords.x = (float)ptDat;

		reader.read((char*)&ptDat, sizeof(short));
		poly->vertecies[c].coords.y = (float)ptDat;
		
		reader.read((char*)&ptDat, sizeof(short));
		poly->vertecies[c].mass = (float)ptDat;

	}

	obj->setCollisionPolyID(objManager::getInstance()->addPoly(poly));

	//Now for weapon.

	bool we = false;

	reader.read((char*)&we, sizeof(bool));

	if(we)
	{
		reader.read((char*)&shorty, sizeof(short));

		((actorObj*)obj)->setWeapon(shorty);

		reader.read((char*)&shorty, sizeof(short));
	}

	//And that concludes file reading.
	reader.close();

	//Time for finishing touches.

	obj->loadAnim(animFile);
	obj->setImgId(viewManager::getInstance()->loadTexture(textureFile, 0xffffffff));


	switch(type)
	{
	case PLAYER:
		OF->registerClass<playerObj>(objectID, obj);
		break;
	case ENEMY:
		OF->registerClass<enemyObj>(objectID, obj);
		break;
	case CRATE:
		OF->registerClass<movingObj>(objectID, obj);
		break;
	case SWITCH:
	case EXIT:
	case PLATFORM:
	default:
		obj->release();
		return 0;
	}

	char* returnVal = new char[objectID.size()+1];
	strcpy_s(returnVal, objectID.size()+1, objectID.c_str());

	return returnVal;
}