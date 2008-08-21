#pragma once

class gameEvent;

class listener
{
public:
	virtual void HandleEvent(gameEvent* ev) = 0;
};