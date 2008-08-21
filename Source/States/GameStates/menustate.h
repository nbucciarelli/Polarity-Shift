#pragma once
#include "..\gameState.h"

class objManager;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class bitFont;
class eventManager;

class menuState : public gameState
{
protected:
	objManager * objM;
	inputDevice* theInput;
	bitFont* theFont;
	eventManager* EM;

	int menuPos, cursorID, menuLast, xPos, yPos;
	char** menuItemString;

	unsigned int textColor, highlightColor;

	virtual void menuHandler() = 0;

	menuState(int x = 100, int y = 100,
		unsigned int color = 0xff66bb66, unsigned int highlight = 0xff88dd88);
	virtual ~menuState(void) {}
public:

	virtual void enter(void);
	virtual void exit(void);

	virtual bool input(float dt);
	virtual void update(float dt);
	virtual void render(void) const;
};
