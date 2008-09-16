#include "enemyObj.h"
#include <cmath>

enemyObj::enemyObj() : actorObj(OBJ_ENEMY)
{}

enemyObj::~enemyObj()
{}

void enemyObj::update(float dt)
{
	actorObj::update(dt);
}