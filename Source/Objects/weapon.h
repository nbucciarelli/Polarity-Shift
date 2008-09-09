#pragma once

typedef unsigned int type;

class baseObj;
union vector3;

enum weaponTypes {
	WEAPON_MAGNET,

	WEAPON_TOTAL
};

class weapon
{
protected:
	type typeID;
	bool isActive; //Weapon being fired, update

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

	static weapon* createWeapon(int weapID, baseObj* receiver = 0);

	type getTypeID() const { return typeID; }
	const baseObj* getOwner() const { return owner; }
	virtual int getRange() const { return range; }
	virtual int getPower() const { return power; }
	virtual int getMode() const { return mode; }
	bool IsActive() const { return isActive; }

	void setOwner(const baseObj* obj) { owner = obj; }
	virtual void setRange(int value) { range = value; }
	virtual void setPower(int value) { power = value; }
	virtual void setMode(int value) { mode = value; }

	virtual void openFire(const vector3& trajectory, int fireMode = 0) = 0;
	virtual void ceaseFire() = 0;

	virtual void update(float dt) {}
	virtual void render() const {}
};