#pragma once

typedef unsigned int type;

class baseObj;
union vector3;

class weapon
{
protected:
	type typeID;
	bool isActive; //Weapon being fired

	const baseObj* owner;

	int range;
	int cooldownTime;
	int cooldown;
	int power;
public:

	weapon(type ID = 0) : typeID(ID), owner(0), range(0), power(0),
						  cooldownTime(0), cooldown(0), isActive(false) {}
	virtual ~weapon() = 0 {}

	type getTypeID() const { return typeID; }
	const baseObj* getOwner() const { return owner; }
	virtual int getRange() const { return range; }
	virtual int getPower() const { return power; }

	void setOwner(const baseObj* obj) { owner = obj; }
	virtual void setRange(int value) { range = value; }
	virtual void setPower(int value) { power = value; }

	virtual void openFire(const vector3& trajectory) = 0;
	virtual void ceaseFire() = 0;

	virtual void update(float dt) {}
	virtual void render() const {}
};