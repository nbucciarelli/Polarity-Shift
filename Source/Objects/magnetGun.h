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
public:
	magnetGun();
	~magnetGun();
	virtual void fire(const vector3& trajectory);

	float getWidthFactor() const { return beamWidthFactor; }

	void setWidthFactor(float value) { beamWidthFactor = value; }
};