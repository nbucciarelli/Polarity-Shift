#pragma once

#include "movingObj.h"

class bulletObj : public movingObj
{
protected:
	const baseObj* owner;
public:
	const baseObj* getOwner() const { return owner; }

	void setOwner(const baseObj* obj) { owner = obj; }
};