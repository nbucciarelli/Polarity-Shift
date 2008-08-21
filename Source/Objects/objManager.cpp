#include "objManager.h"
#include "baseObj.h"

objManager::objManager(void)
{
}

objManager::~objManager(void)
{
}

objManager* objManager::getInstance()
{
	static objManager ObiWan;
	return &ObiWan;
}

void objManager::update(float dt)
{
	for(unsigned c = 0; c < objList.size(); c++)
		objList[c]->update(dt);
}

void objManager::render() const
{
	for(unsigned c = 0; c < objList.size(); c++)
		objList[c]->render();
}

//Add an object
void objManager::addObj(baseObj* obj)
{
	obj->acquire();

	objList.push_back(obj);
}

//Remove an object
void objManager::removeObj(baseObj* obj)
{
	if(!obj)
		return;

	for (vector<baseObj*>::iterator iter = objList.begin();
		 iter != objList.end(); iter++)
	{
		if ((*iter) == obj)
		{
			(*iter)->release();

			objList.erase(iter);
			break;
		}
	}
}

//Remove all objects
void objManager::clear()
{
	for(unsigned c = 0; c < objList.size(); c++)
		objList[c]->release();

	objList.clear();
}

void objManager::checkCollisions()
{
	for(unsigned c = 0; c < objList.size(); c++)
	{
		//Do not do collision checking on BG objects.
//		if(objList[c]->getType() == OBJ_BG)
//			continue;

		for(unsigned d = 0; d < objList.size(); d++)
		{
//			if(objList[d]->getType() == OBJ_BG)
//				continue;
			objList[c]->checkCollision(objList[d]);
		}
	}
}