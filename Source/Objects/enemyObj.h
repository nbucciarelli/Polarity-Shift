#pragma once

#include "actorObj.h"

enum enemyTypes {
	ET_COUNT,
};

class enemyObj : public actorObj
{
protected:
	int enemyType;
public:
	enemyObj();
	~enemyObj();

	void update(float dt);
	int getEnemyType() const { return enemyType; }
	void setEnemyType(int value) { enemyType = value; }
};