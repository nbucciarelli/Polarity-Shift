#include "criticalSection.h"
#include <windows.h>

criticalSectionControl::criticalSectionControl()
{}

criticalSectionControl::~criticalSectionControl()
{}

criticalSectionControl* criticalSectionControl::getInstance()
{
	static criticalSectionControl Zod;

	return &Zod;
}

unsigned int criticalSectionControl::getCriticalSection()
{
	//Check for a released critical section
	for(unsigned c = 0; c < criticals.size(); c++)
	{
		if(criticals[c]->active)
			continue;
		
		criticals[c]->active = true;
		return c;
	}

	//If we haven't found one, then make a new one.
	criticalSection* holder = new criticalSection;

	holder->active = true;
	holder->locked = false;

	criticals.push_back(holder);

	return (unsigned)(criticals.size() - 1);
}

void criticalSectionControl::releaseCriticalSection(unsigned int id)
{
	//Paranoia check
	if(id >= criticals.size())
		return;

	criticals[id]->active = false;
}

void criticalSectionControl::lockSection(unsigned int id)
{
	criticals[id]->locked = true;
}

void criticalSectionControl::unlockSection(unsigned int id)
{
	criticals[id]->locked = false;
}

bool criticalSectionControl::isLocked(unsigned int id)
{
	return criticals[id]->locked;
}

void criticalSectionControl::waitForUnlock(unsigned int id)
{
	while(criticals[id]->locked)
		Sleep(0);
}