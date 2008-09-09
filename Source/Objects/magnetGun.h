#pragma once

#include "weapon.h"

class movingObj;

enum magMode {
	MAG_OFF,
	MAG_PUSH,
	MAG_PULL,
	MAG_HOLD,

	MAG_MODES
};

class mouse;

class magnetGun : public weapon
{
protected:
	float beamWidthFactor;
	mouse* theMouse;

	bool getTarget(const vector3& farPoint);

	movingObj* target;

	int mode;
public:
	magnetGun();
	~magnetGun();

	virtual void openFire(const vector3* trajectory = 0, int fireMode = 0);
	virtual void ceaseFire();

	float getWidthFactor() const { return beamWidthFactor; }

	void setWidthFactor(float value) { beamWidthFactor = value; }

	void update(float dt);
	void render() const;
};