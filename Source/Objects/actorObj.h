#pragma once
#include "movingObj.h"
#include "../eventsystem/listener.h"

class weapon;

class actorObj : public movingObj, public listener
{
protected:
	float maxAcc;
	float maxVel;

	int health;

	float accStep;

	weapon* theWeapon;
public:
	actorObj(unsigned type = OBJ_ACTOR);
	actorObj(const actorObj&);
	~actorObj();

	virtual void update(float dt);

	virtual void HandleEvent(gameEvent* ev);

	void setWeapon(int weapID);
	void setAccStep(float val) { accStep = val; }
	void setMaxAcc(float val) { maxAcc = val; }
	void setMaxVel(float val) { maxAcc = val; }

	weapon* getWeapon() const { return theWeapon; }
	float getAccStep() const { return accStep; }
	float getMaxAcc() const { return maxAcc; }
	float getMaxVel() const { return maxVel; }
};