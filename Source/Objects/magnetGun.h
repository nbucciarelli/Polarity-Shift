#pragma once

#include "weapon.h"

enum magMode {
	MAG_OFF,
	MAG_PUSH,
	MAG_PULL,
	MAG_HOLD,

	MAG_MODES
};

class magnetGun : public weapon
{
protected:
	float beamWidthFactor;

	baseObj* getTarget(const vector3& farPoint);

	int mode;
public:
	magnetGun();
	~magnetGun();

	virtual int getMode() const { return mode; }
	virtual void setMode(int value) { mode = value; }

	virtual void openFire(const vector3& trajectory);
	virtual void ceaseFire();

	float getWidthFactor() const { return beamWidthFactor; }

	void setWidthFactor(float value) { beamWidthFactor = value; }

	void update(float dt);
	void render() const;
};