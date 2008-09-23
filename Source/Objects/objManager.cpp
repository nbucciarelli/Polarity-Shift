#include "objManager.h"
#include "baseObj.h"
#include "../eventsystem/eventmanager.h"
#include "../eventsystem/eventids.h"

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
		if(objList[c]->getIsActive())
			objList[c]->update(dt);
		else
			eventManager::getInstance()->sendEvent(EVENT_KILLOBJ, objList[c]);
}

void objManager::render() const
{
	for(unsigned c = 0; c < objList.size(); c++)
		if(objList[c]->getIsActive())
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

	while(polyList.size())
	{
		delete polyList.back();
		polyList.pop_back();
	}
}

void objManager::checkCollisions()
{
	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(objList[c]->getIsActive())
			objList[c]->mapCollisionCheck();
	}

	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(objList[c]->getIsActive())
			for(unsigned d = 0; d < objList.size(); d++)
			{
				if(c == d)
					continue;

				if(objList[d]->getIsActive())
					objList[c]->checkCollision(objList[d]);
			}
	}

	for(unsigned c = 0; c < objList.size(); c++)
	{
		if(objList[c]->getIsActive())
			objList[c]->collisionReact();
	}
}

int objManager::addPoly(polygon* p)
{
	polyList.push_back(p);

	return (int)polyList.size() - 1;
}

polygon* objManager::getPoly(int id)
{
	if(id < 0 || (unsigned)id > polyList.size())
		return 0;

	return polyList[id];
}