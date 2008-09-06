#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �gamePlayState.h�
// Author: Scott Smallback (SS)
// Purpose: This is the header file for the game play state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "../gameState.h"

class objManager;
class CSGD_DirectInput;
typedef CSGD_DirectInput inputDevice;
class eventManager;
class playHandler;
class CTileEngine;

class gamePlayState : public gameState
{
protected:
	CTileEngine* TE;
	objManager * OM;
	inputDevice* theInput;
	eventManager* EM;
	playHandler* handler;

	volatile bool rendering;

	gamePlayState();
	gamePlayState(const gamePlayState&);
	gamePlayState& operator=(const gamePlayState&);
	~gamePlayState();

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �getInstance�
	// Last modified: August 27, 2008
	// Purpose: This gets the instance of the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static gamePlayState* getInstance();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �enter�
	// Last modified: August 27, 2008
	// Purpose: Enters the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void enter(void);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �exit�
	// Last modified: August 27, 2008
	// Purpose: Exits the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void exit(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �input�
	// Last modified: August 27, 2008
	// Purpose: Input used for the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool input(float dt);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �update�
	// Last modified: August 27, 2008
	// Purpose: Updates the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void update(float dt);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �render�
	// Last modified: August 27, 2008
	// Purpose: Renders the game play state
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render(void) const;

};