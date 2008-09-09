#include "weapon.h"
#include "magnetGun.h"

weapon* weapon::createWeapon(int weapID, baseObj* receiver)
{
	weapon* holder = 0;
	switch(weapID)
	{
	case WEAPON_MAGNET:
		holder = new magnetGun;
	default:
		return 0;
	}

	holder->setOwner(receiver);

	return holder;
}