#pragma once

typedef unsigned int mID;

class message
{
	mID id;
protected:
public:
	message(mID msg = 0) { id = msg; }
	virtual ~message(void) {}

	mID getID(void) { return id; }
};