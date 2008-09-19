#pragma once

#include "actorObj.h"

enum enemyTypes { ET_SPIDER, ET_TURRET,
	ET_COUNT,
};

class enemyObj : public actorObj
{
protected:
	int enemyType, m_nExplosionID, m_nExpX, m_nExpY;
	bool m_bDied;
public:
	enemyObj();
	~enemyObj();

	void update(float dt);
	void render();
	int getEnemyType() const { return enemyType; }
	void setEnemyType(int value) { enemyType = value; }

	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);
};