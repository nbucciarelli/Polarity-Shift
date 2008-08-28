#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �playerObj.h�
// Author: Scott Smallback (SS)
// Purpose: This is the header file for the object that will be made into the player
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "movingObj.h"
#include "../EventSystem/listener.h"

//class CAnimationManager;

class playerObj : public listener, public movingObj
{
protected:
	int jumpTime;
	float maxJumpTime;
	bool jumpDone;

	float maxAcc;
	float maxVel;

	float accStep;

	int range;
	int nAnimNumber;

	//CAnimationManager* m_pAM;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Constructor�
	// Last Modified: August 27, 2008
	// Purpose: This is the constructor for the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	playerObj();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Deconstructor�
	// Last Modified: August 27, 2008
	// Purpose: This is the deconstructor for the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~playerObj();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �HandleEvent�
	// Last Modified: August 27, 2008
	// Purpose: This handles events for the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void HandleEvent(gameEvent *ev);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Update�
	// Last Modified: August 27, 2008
	// Purpose: This is the update for the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void update(float dt);
	//void render(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Mutators�
	// Last Modified: August 27, 2008
	// Purpose: This is the setting information in the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void setAccStep(float step) { accStep = step; }
	void SetAnimNumber(int val) { nAnimNumber = val; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: �Accessor�
	// Last Modified: August 27, 2008
	// Purpose: This is the getting information in the player object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetAnimNumber() const { return nAnimNumber; }
};