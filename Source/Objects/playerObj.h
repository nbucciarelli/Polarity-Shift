#pragma once
#include "movingObj.h"
#include "../EventSystem/listener.h"

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
public:
	playerObj();
	~playerObj();

	void HandleEvent(gameEvent *ev);

	void update(float dt);

	void setAccStep(float step) { accStep = step; }
};