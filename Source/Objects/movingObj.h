#pragma once
#include "baseObj.h"

class movingObj : public baseObj
{
protected:
	vector3 velocity, acceleration, angVel, angAcc;
	
	const rect* boundingSpace;

	//Keeps the object in bounds
	virtual void boundsCheck();

public:
	movingObj(uint otype = OBJ_MOVING);
	virtual ~movingObj(void);

	virtual bool checkCollision(baseObj* obj, rect* _unused = NULL);

	void update(float dt);

	//Accessors
	const vector3& getVelocity() const { return velocity; }
	const vector3& getAcceleration() const { return acceleration; }
	const vector3& getAngVel() const { return angVel; }
	const vector3& getAngAcc() const { return angAcc; }
	const rect& getBounds() const { return *boundingSpace; }

	//Set Mutators: variable = input
	void setVel(const vector3& vel) { velocity = vel; }
	void setVelX(float v) { velocity.x = v; }
	void setAcc(const vector3& acc) { acceleration = acc; }
	void setAngVel(const vector3& aVel) { angVel = aVel; }
	void setAngAcc(const vector3& aAcc) { angAcc = aAcc; }
	void clearRotData() { angPos = angVel = angAcc = vector3(); }
	void setBounds(const rect * const bound) { boundingSpace = bound; }

	//Mod Mutators: variable += input
	void modVel(const vector3& change) { velocity += change; }
	void modAcc(const vector3& change) { acceleration += change; }
	void modAngVel(const vector3& change) { angVel += change; }
	void modAngAcc(const vector3& change) { angAcc += change; }
};
