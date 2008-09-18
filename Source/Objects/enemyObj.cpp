#include "enemyObj.h"
#include <cmath>
#include "../helpers/datatypes.h"

enemyObj::enemyObj() : actorObj(OBJ_ENEMY)
{}

enemyObj::~enemyObj()
{}

void enemyObj::update(float dt)
{
	actorObj::update(dt);
}

bool enemyObj::checkCollision(baseObj* obj, polyCollision* result)
{
	polyCollision holder;

	actorObj::checkCollision(obj, &holder);



	return holder.overlapped || holder.willCollide;
}