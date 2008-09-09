#include "mouse.h"
#include "viewManager.h"
#include "CSGD_DirectInput.h"
#include "../helpers/physics.h"
#include "../helpers/criticalsection.h"

mouse::mouse() : imgID(-1), theInput(CSGD_DirectInput::GetInstance())
{}

mouse::~mouse()
{}

void mouse::initialize()
{ CRITICAL_INIT; }

void mouse::shutdown()
{ CRITICAL_RELEASE; }

mouse* mouse::getInstance()
{
	static mouse mickey;

	return &mickey;
}

void mouse::update(float dt)
{
	CRITICAL({
	position += vector3((float)theInput->MouseMovementX(),
						(float)theInput->MouseMovementY(), 0);
	});
}

void mouse::render() const
{
	if(imgID == -1)
		return;

	CRITICAL({
		viewManager::getInstance()->drawTexture(imgID,
			&(position - clickPos), 0, 0, 0, 0xff000000);
	});
}

void mouse::setImgID(int val)
{
	CRITICAL({
	if(imgID != -1)
		viewManager::getInstance()->releaseTexture(imgID);

	imgID = val;

	viewManager::getInstance()->acquireTexture(imgID);
	});
}