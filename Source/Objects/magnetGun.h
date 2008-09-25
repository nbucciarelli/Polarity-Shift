#pragma once

#include "weapon.h"
#include "../helpers/datatypes.h"
#include "../helpers/criticalsectionmacros.h"

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
	vector3 pos;
	vector3 aimVect;
	matrix worldMatrix;

	bool getTarget(const vector3& farPoint);
	//float levelLimiter(const vector3& traj);

	CRITICAL_VARS;

	movingObj* target;

	int mode;
	int imgId;
	pt imgCenter;

	void updateWorldMatrix();

public:
	magnetGun();
	~magnetGun();

	virtual void openFire(const vector3* trajectory = 0, int fireMode = 0);
	virtual void ceaseFire(int dat = 0);

	float getWidthFactor() const { return beamWidthFactor; }

	void setWidthFactor(float value) { beamWidthFactor = value; }

	void update(float dt);
	void render() const;
};