#pragma once
#include <vector>

class criticalSectionControl
{
protected:

	struct criticalSection
	{
		volatile bool locked;
		bool active;
	};

	std::vector<criticalSection*> criticals;

	//Singleton protection
	criticalSectionControl();
	criticalSectionControl(const criticalSectionControl&);
	criticalSectionControl& operator=(const criticalSectionControl&);
	~criticalSectionControl();
public:
	static criticalSectionControl* getInstance();

	unsigned int getCriticalSection();
	void releaseCriticalSection(unsigned int id);

	void lockSection(unsigned int id);
	void unlockSection(unsigned int id);

	bool isLocked(unsigned int id);

	void waitForUnlock(unsigned int id);
};

#define CRITICAL(fun)  {\
	CS->waitForUnlock(CSID);\
	CS->lockSection(CSID); \
	fun; \
	CS->unlockSection(CSID); \
	}