#pragma once

typedef unsigned int EvID;

class gameEvent
{
protected:
	EvID id;
	void* data;
public:
	gameEvent(EvID eventID, void* dat = 0)
	{
		id = eventID;
		data  = dat;
	}

	EvID getEventID(void) { return id; }
	void* getData(void) { return data; }
};