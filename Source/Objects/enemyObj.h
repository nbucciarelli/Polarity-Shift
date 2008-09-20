#pragma once

#include "actorObj.h"

enum enemyTypes { ET_SPIDER, ET_TURRET, ET_BOSS, ET_COUNT, ET_SCORE, ET_INV};

class enemyObj : public actorObj
{
protected:
	int enemyType, m_nImageID, m_nExpX, m_nExpY;
	bool m_bDied;
	int m_nHP;
public:
	enemyObj(int type = ET_SPIDER);
	~enemyObj();

	void update(float dt);
	void render();
	int getEnemyType() const { return enemyType; }
	int getHP() {return m_nHP;}
	void setEnemyType(int value) { enemyType = value; }

	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);
};