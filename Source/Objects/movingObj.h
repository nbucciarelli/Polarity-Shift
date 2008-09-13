#pragma once
#include "baseObj.h"

struct polygon;

class movingObj : public baseObj
{
protected:
	vector3 velocity, acceleration, angVel, angAcc;

	bool onSurface, leftWall, rightWall, topWall;
	virtual bool mapCollisionCheck();

	bool collisionHandling(const polygon& poly, polyCollision& result, baseObj* obj = NULL);
	
public:
	movingObj(uint otype = OBJ_MOVING);
	virtual ~movingObj(void);

	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);

	void update(float dt);

	//Accessors
	const vector3& getVelocity() const { return velocity; }
	const vector3& getAcceleration() const { return acceleration; }
	const vector3& getAngVel() const { return angVel; }
	const vector3& getAngAcc() const { return angAcc; }

	//Set Mutators: variable = input
	void setVel(const vector3& vel) { velocity = vel; }
	void setVelX(float v) { velocity.x = v; }
	void setAcc(const vector3& acc) { acceleration = acc; }
	void setAngVel(const vector3& aVel) { angVel = aVel; }
	void setAngAcc(const vector3& aAcc) { angAcc = aAcc; }
	void clearRotData() { angPos = angVel = angAcc = vector3(); }

	//Mod Mutators: variable += input
	void modVel(const vector3& change) { velocity += change; }
	void modAcc(const vector3& change) { acceleration += change; }
	void modAngVel(const vector3& change) { angVel += change; }
	void modAngAcc(const vector3& change) { angAcc += change; }
};
