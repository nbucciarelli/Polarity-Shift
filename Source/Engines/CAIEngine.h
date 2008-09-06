#pragma once
#include "../EventSystem/listener.h"
#include <vector>
using std::vector;

class enemyObj;
class playerObj;
class eventManager;

class CAIEngine: public listener
{
private:
	vector<enemyObj*> enemyList;
	playerObj* player;
	eventManager* EM;



public:
	CAIEngine();

	~CAIEngine();

	void update();
	void HandleEvent(gameEvent *ev);
};