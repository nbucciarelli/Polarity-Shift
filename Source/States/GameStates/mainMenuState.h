#pragma once
#include "menuState.h"

class mainMenuState : public menuState
{
protected:
	enum menuOptions { PLAY, OPTIONS, HOWTO, SCORES, CREDITS,
		
		EXIT, TOTAL
	};

	void menuHandler();

	mainMenuState(void);
	mainMenuState(const mainMenuState&);
	mainMenuState& operator=(const mainMenuState&);
	virtual ~mainMenuState(void);
public:

	static mainMenuState* getInstance();
	
	virtual void enter(void);
	virtual void exit(void);

	virtual void render(void) const;
};