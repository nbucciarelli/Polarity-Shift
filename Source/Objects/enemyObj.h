#pragma once

#include "movingObj.h"
#include "../EventSystem/listener.h"

enum enemyTypes {
	ET_COUNT,
};

class enemyObj : /*public listener,*/ public movingObj
{
protected:
	int enemyType;
	float maxAcc;
	float maxVel;
public:
	enemyObj();
	~enemyObj();

	void update(float dt);
	int getEnemyType() const { return enemyType; }
	void setEnemyType(int value) { enemyType = value; }
};