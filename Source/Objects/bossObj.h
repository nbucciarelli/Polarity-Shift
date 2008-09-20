#pragma once

#include "actorObj.h"

class bossObj : public actorObj
{
protected:
	int hp;
public:
	bossObj();
	~bossObj();

	void update(float dt);
	void render();

	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);
};