#pragma once

typedef unsigned int type;

class baseObj;
union vector3;

enum weaponTypes {
	WEAPON_MAGNET,
	WEAPON_PROJECTILE,
	WEAPON_BEAM,

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
	weapon(type ID = 0) : typeID(ID), owner(0), range(800), power(200),
						  cooldownTime(0), cooldown(0), isActive(false) {}
	virtual ~weapon() = 0 {}

	static weapon* createWeapon(int weapID, baseObj* receiver = 0);
	void release();

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

	virtual void openFire(const vector3* trajectory = 0, int fireMode = 0) = 0;
	virtual void ceaseFire(int dat = 0) = 0;

	virtual void update(float dt) {}
	virtual void render() const {}
};