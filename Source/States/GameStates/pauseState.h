#pragma once
#include "menuState.h"

class eventHandler;

class pauseState : public menuState
{
protected:
	enum menuOptions { RESUME, 
		
		EXIT, TOTAL };

	void menuHandler();



	//Singleton protection
	pauseState(void);
	pauseState(const pauseState&);
	pauseState& operator=(const pauseState&);
	~pauseState(void);
public:

	static pauseState* getInstance();

	void enter(void);
	void exit(void);

	bool input(float dt);
	void update(float dt);
	void render(void) const;
};
