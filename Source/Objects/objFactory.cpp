#include "objFactory.h"
#include "baseObj.h"

objFactory::objFactory(void)
{
}

objFactory::~objFactory(void)
{
	unregisterAll();
}

objFactory* objFactory::getInstance()
{
	static objFactory Camino;
	return &Camino;
}

void objFactory::unregisterClass(IDType id)
{
	for(vector<creationEntry>::iterator iter = creationList.begin();
		iter != creationList.end(); iter++)
	{
		if ((*iter).id == id)
		{
			if((*iter)._template)
				iter->_template->release();

			creationList.erase(iter);

			break;
		}
	}
}

void objFactory::unregisterAll()
{
	for(vector<creationEntry>::iterator iter = creationList.begin(); iter != creationList.end(); iter++)
	{
		if(iter->_template)
			//iter->_template->release();
			delete iter->_template;
	}
	creationList.clear();
}

baseObj* objFactory::spawn(IDType id)
{
	for(vector<creationEntry>::iterator iter = creationList.begin();
		iter != creationList.end(); iter++)
	{
		if ((*iter).id == id)
		{
			return (*iter).creator((*iter)._template);
		}
	}
	return NULL;
}