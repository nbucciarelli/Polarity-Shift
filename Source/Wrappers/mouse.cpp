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

void mouse::setPos(const vector3& val)
{
	CRITICAL({position = val;});
}

void mouse::update(float dt)
{
	CRITICAL({
	position += vector3((float)theInput->MouseMovementX(),
						(float)theInput->MouseMovementY(), 0);

	if(position.x < 0)
		position.x = 0;
	if(position.y < 0)
		position.y = 0;
	if(position.x > 1024)
		position.x = 1024;
	if(position.y > 600)
		position.y = 600;
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