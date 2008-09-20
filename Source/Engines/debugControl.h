#pragma once

#include <vector>
#include "../helpers/datatypes.h"
#include "../eventsystem/listener.h"

class objManager;
class viewManager;
class renderer;
class CTileEngine;

class debugControl : public listener
{
protected:
	objManager* OM;
	viewManager* VM;
	CTileEngine* TE;
	renderer* Re;
	volatile bool ready;
	volatile bool locked;

	volatile int uCount;
	volatile int rCount;

	rect dot;

	//I was going to do something more...but yeah.
	struct drawGroup
	{
		std::vector<vector3> pts;

		drawGroup() {}
	};

	std::vector<drawGroup> lineGroups;
	std::vector<drawGroup> mapLines;
	std::vector<drawGroup> magLines;
	std::vector<vector3> posPts;

	void getMapCollisions();

public:
	debugControl();
	~debugControl();

	void initialize();
	void shutdown();

	void update(float dt);
	void render();

	void HandleEvent(gameEvent* ev);
};