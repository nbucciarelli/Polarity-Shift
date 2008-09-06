#pragma once

#include "movingObj.h"

enum enemyTypes {
	ET_COUNT,
};

class enemyObj : public movingObj
{
protected:
	int enemyType;
public:
	enemyObj();
	~enemyObj();

	int getEnemyType() const { return enemyType; }
	void setEnemyType(int value) { enemyType = value; }
};