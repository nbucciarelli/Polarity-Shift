#pragma once
#include "movingObj.h"
#include "../EventSystem/listener.h"

class playerObj : public movingObj, public listener
{
protected:
	float jumpTime;
	float maxJumpTime;

	float maxAcc;
	float maxVel;

	float accStep;

	int range;
public:
	playerObj();
	~playerObj();

	void HandleEvent(gameEvent *ev);

	void update(float dt);
};