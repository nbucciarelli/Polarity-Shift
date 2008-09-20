#pragma once

#include "actorObj.h"

enum enemyTypes { ET_SPIDER, ET_TURRET, ET_BOSS, ET_COUNT};

class enemyObj : public actorObj
{
protected:
	int enemyType;
	bool m_bDied;
	int m_nHP;
public:
	enemyObj(int type = ET_SPIDER);
	~enemyObj();

	void update(float dt);
	void render();
	int getEnemyType() const { return enemyType; }
	void setEnemyType(int value) { enemyType = value; }

	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);
};