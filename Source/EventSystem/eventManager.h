#pragma once

#include "gameEvent.h"
#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

class listener;

typedef multimap<EvID, listener*> clientMap;

class eventManager
{
protected:
	//'client' listing.  Who wants what event.
	clientMap clientList;

	//event processing stack
	list<gameEvent> currentEvents;
	list<gameEvent> globalEvents;

	//Singleton protection
	eventManager(void);
	eventManager(const eventManager&);
	eventManager& operator=(const eventManager&);

	bool isRegistered(EvID id, listener* client);
	void runEvent(gameEvent* theEvent);
public:
	~eventManager(void);

	static eventManager* getInstance();
	
	void shutdown(void);

	//Adds a client to an event watch.  Adds the event if it's not in the map.
	void registerClient(EvID eventID, listener* client);

	//removes client from the given event
	void unregisterClient(EvID eventID, listener* client);

	//removes client from all events it's watching
	//Note that this is less efficient than specifics
	void unregisterClient(listener* client);

	//adds an event to the processing stack
	void sendEvent(EvID eventID, void* data = NULL);
	void sendGlobalEvent(EvID eventID, void* data = NULL);

	//go through the events stack
	void processEvents(void);
	void processGlobalEvents();

	//dumps event stack
	void clearLocalEvents(void);
	void clearGlobalEvents();
	void clearAllEvents();
};
