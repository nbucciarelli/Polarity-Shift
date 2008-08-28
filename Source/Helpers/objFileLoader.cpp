#include "objFileLoader.h"
#include <windows.h>

#include <fstream>
using std::ifstream;

#include "../Objects/objFactory.h"
#include "../Wrappers/viewManager.h"
#include "datatypes.h"

#include "../Objects/playerObj.h"

objFileLoader::objFileLoader()
{
	OF = objFactory::getInstance();
}

objFileLoader::~objFileLoader()
{
}

void objFileLoader::loadObject(char* filename)
{
	ifstream reader;

	reader.open(filename);

	if(reader.fail())
	{
		MessageBox(NULL, "Error Loading Game Object.", "Error", MB_OK);
		return;
	}

	int holder = 0;

	//Grab, ignore, version information
	reader.read((char*)&holder, sizeof(float));

	//Grab type.
	reader.read((char*)&holder, sizeof(short));

	short type = *((short*)&holder);

	//Grab name length.
	reader.read((char*)&holder, sizeof(char));

	char textbuffer[100] = {0};

	//Read name, assign as factory designator
	reader.read(textbuffer, *((char*)&holder));
	IDType objectID = textbuffer;

	memset(textbuffer, 0, sizeof(textbuffer));

	//A pointer to do stuff.
	baseObj* obj = NULL;

	switch(type)
	{
	case PLAYER:
		obj = new playerObj;
		OF->registerClass<playerObj>(objectID, obj);
		break;
	case ENEMY:
	case SWITCH:
	case EXIT:
	case PLATFORM:
		return;
	case CRATE:
		obj = new movingObj;
		OF->registerClass<movingObj>(objectID, obj);
		break;
	default:
		return;
	}

	//grab texture path length
	reader.read((char*)&holder, sizeof(char));

	reader.read(textbuffer, *((char*)&holder));

	char text2[100] = {0};

	sprintf(text2, "Resource/%s", textbuffer);

	obj->setImgId(viewManager::getInstance()->loadTexture(text2));

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
	obj->release();
}