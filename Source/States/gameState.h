#pragma once

//gameState:  abstract base for primary gamestates.

class gameState
{
public:
	virtual ~gameState(void) = 0 {}
	
	virtual void enter(void) = 0;
	virtual void exit(void) = 0;

	virtual bool input(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void render(void) const = 0;
};