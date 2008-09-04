#pragma once

typedef unsigned int type;

class baseObj;
union vector3;

class weapon
{
protected:
	type typeID;

	const baseObj* owner;

	int range;
	int cooldownTime;
	int cooldown;
	int power;
	int mode;
public:

	weapon(type ID = 0) : typeID(ID), owner(0), range(0), power(0),
						  cooldownTime(0), cooldown(0) {}
	virtual ~weapon() = 0 {}

	type getTypeID() const { return typeID; }
	const baseObj* getOwner() const { return owner; }
	virtual int getRange() const { return range; }
	virtual int getPower() const { return power; }
	virtual int getMode() const { return mode; }

	void setOwner(const baseObj* obj) { owner = obj; }
	virtual void setRange(int value) { range = value; }
	virtual void setPower(int value) { power = value; }
	virtual void setMode(int value) { mode = value; }

	virtual void fire(const vector3& trajectory) = 0;
};