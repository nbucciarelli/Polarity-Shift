#include "camera.h"
#include "..\EventSystem\eventIDs.h"
#include "..\EventSystem\gameEvent.h"
#include "..\Helpers\physics.h"
#include "..\Objects\baseObj.h"

camera::camera() {}
camera::~camera() {}

camera* camera::getInstance()
{
	static camera news;
	return &news;
}

void camera::initialize(const vector3& tracking)
{
	trackPos = tracking;
}

void camera::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case EVENT_PLAYERLOADED:
		track = (baseObj*)ev->getData();
		break;
	case EVENT_PLAYERKILLED:
		if(track == ev->getData())
			track = 0;
	default:
		break;
	}
}

void camera::update(float dt)
{
	if(!track)
		return;

	camPos = track->getPosition() - trackPos;

	if(camPos.x < 0)
		camPos.x = 0;

	if(camPos.x + 800 > bounds->right)
		camPos.x = (float)bounds->right - 800;

	if(camPos.y + 600 > bounds->bottom + 64)
		camPos.y = (float)bounds->bottom -536;

	//I have absolutely NO idea why this requires a -2
	calc::matrixTranslate(viewMatrix, camPos * -2);
}