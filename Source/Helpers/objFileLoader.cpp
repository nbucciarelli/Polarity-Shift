#include "objFileLoader.h"
#include <windows.h>

#include <fstream>
using std::ifstream;

#include "../Objects/objFactory.h"
#include "../Wrappers/viewManager.h"
#include "datatypes.h"

#include "../Objects/playerObj.h"
#include "../Objects/enemyObj.h"

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

const char* objFileLoader::loadObject(char* filename)
{
	ifstream reader;

	reader.open(filename);

	if(reader.fail())
	{
		MessageBox(NULL, "Error Loading Game Object.", "Error", MB_OK);
		return 0;
	}

	int holder = 0;

	//Grab, ignore, version information
	reader.read((char*)&holder, sizeof(float));

	//Grab type.
	reader.read((char*)&holder, sizeof(short));

	short type = *((short*)&holder);

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

	//Grab name length.
	reader.read((char*)&holder, sizeof(char));

	char textbuffer[100] = {0};

	//Read name, assign as factory designator
	reader.read(textbuffer, *((char*)&holder));
	IDType objectID = textbuffer;

	memset(textbuffer, 0, sizeof(textbuffer));

	//grab texture path length
	reader.read((char*)&holder, sizeof(char));

	reader.read(textbuffer, *((char*)&holder));

	obj->setImgId(viewManager::getInstance()->loadTexture(&textbuffer[1]));

	//Currently unused data.
	reader.read((char*)&holder, sizeof(int));
	reader.read((char*)&holder, sizeof(int));

	//Now grab point count
	reader.read((char*)&holder, sizeof(short));

	polygon * poly = new polygon;

	poly->vertexCount = *((short*)&holder);
	poly->vertecies = new objectPoint[*((short*)&holder)];

	for(int c = 0; c < *((short*)&holder); c++)
	{
		reader.read((char*)&holder, sizeof(int));

		poly->vertecies[c].coords.x = (float)((short*)&holder)[0];
		poly->vertecies[c].coords.y = (float)((short*)&holder)[1];
		
		reader.read((char*)&holder, sizeof(short));

		poly->vertecies[c].mass = (float)(*((short*)&holder));
	}

	obj->setCollisionPoly(poly);

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
		delete poly;
		delete obj;
		return 0;
	}

	obj->release();

	char* returnVal = new char[objectID.size()+1];
	strcpy_s(returnVal, objectID.size()+1, objectID.c_str());

	return returnVal;
}