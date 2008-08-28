//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"eventManager.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	Manages the messages
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "eventManager.h"
#include "listener.h"

typedef pair<clientMap::iterator, clientMap::iterator> rangePair;

#pragma region constructor / destructor
eventManager::eventManager(void) {}

eventManager::~eventManager(void) {}
#pragma endregion

#pragma region singleton & shutdown
eventManager* eventManager::getInstance()
{
	static eventManager GLaDDOS;
	return &GLaDDOS;
}

void eventManager::shutdown(void)
{
	currentEvents.clear();
	clientList.clear();
}
#pragma endregion

#pragma region registration
bool eventManager::isRegistered(EvID id, listener* client)
{
	//These range things of multi-map are pretty cool, actually.
	rangePair range = clientList.equal_range(id);

	for(clientMap::iterator iter = range.first; iter != range.second; iter++)
	{
		if((*iter).second == client)
		{
			return true;
			break;
		}
	}
	return false;
}

void eventManager::registerClient(EvID eventID, listener* client)
{
	//Do nothing if this is a repeat
	if (!client || isRegistered(eventID, client))
		return;

	//else stick it in
	clientList.insert( std::make_pair(eventID, client) );
}
#pragma endregion

#pragma region unregistration
void eventManager::unregisterClient(EvID eventID, listener* client)
{
	rangePair range = clientList.equal_range(eventID);

	for(clientMap::iterator iter = range.first; iter != range.second; iter++)
	{
		//Once we find the client, get rid.
		if((*iter).second == client)
		{
			iter = clientList.erase(iter);
			break;
		}
	}
}

void eventManager::unregisterClient(listener* client)
{
	clientMap::iterator iter = clientList.begin();

	while(iter != clientList.end())
	{
		if((*iter).second == client)
		{
			iter = clientList.erase(iter);
		}
		else
			iter++;
	}
}
#pragma endregion

#pragma region eventHandling
void eventManager::sendEvent(EvID eventID, void* data)
{
	currentEvents.push_back(gameEvent(eventID, data));
}

void eventManager::sendGlobalEvent(EvID eventID, void* data)
{
	globalEvents.push_back(gameEvent(eventID, data));
}

void eventManager::processEvents(void)
{
	while(!currentEvents.empty())
	{
		runEvent(&currentEvents.front());
		currentEvents.pop_front();
	}
}
void eventManager::processGlobalEvents(void)
{
	while(!globalEvents.empty())
	{
		runEvent(&globalEvents.front());
		globalEvents.pop_front();
	}
}

void eventManager::clearLocalEvents(void)
{
	currentEvents.clear();
}

void eventManager::clearGlobalEvents(void)
{
	globalEvents.clear();
}

void eventManager::clearAllEvents(void)
{
	clearLocalEvents();
	clearGlobalEvents();
}

void eventManager::runEvent(gameEvent* theEvent)
{
	rangePair range = clientList.equal_range(theEvent->getEventID());

	for(clientMap::iterator iter = range.first;
					iter != range.second; iter++)
	{
		(*iter).second->HandleEvent(theEvent);
	}
}
#pragma endregion