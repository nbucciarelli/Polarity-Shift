////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "criticalSelectionMacros.h"
// Author: Scott Smallback(SS)
// Purpose: Macros for the sleection process
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class criticalSectionControl;

#define CRITICAL(fun)  {\
	CS->waitForUnlock(CSID);\
	CS->lockSection(CSID); \
	fun; \
	CS->unlockSection(CSID); \
	}

#define CRITICAL_SKIP(fun) {\
	if(!CS->isLocked(CSID))\
	{\
	CS->lockSection(CSID); \
	fun; \
	CS->unlockSection(CSID); \
	}\
	}

#define CRITICAL_VARS criticalSectionControl * CS;\
	unsigned int CSID

#define CRITICAL_INIT CS = criticalSectionControl::getInstance();\
	CSID = CS->getCriticalSection()

#define CRITICAL_RELEASE CS->releaseCriticalSection(CSID)