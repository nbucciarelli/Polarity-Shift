//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:			"playerHandler.cpp"
//	Author:			Scott Smallback (SS)
//	Purpose:		The event system for the player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "playHandler.h"
#include "../Objects/objFactory.h"
#include "../Objects/objManager.h"
#include "../Wrappers/viewManager.h"
#include "eventManager.h"
#include "eventIDs.h"
#include "globalEvents.h"
#include "../Helpers/objFileLoader.h"

#include "../Objects/playerObj.h"

void playHandler::initialize()
{
	EM = eventManager::getInstance();
	OM = objManager::getInstance();
	OF = objFactory::getInstance();
	FL = new objFileLoader;

	for(int c = BEGIN_HANDLER_EVENTS + 1; c < END_HANDLER_EVENTS; c++)
	{
		EM->registerClient(c, this);
	}
}

void playHandler::shutdown()
{
	delete FL;

	for(int c = BEGIN_HANDLER_EVENTS + 1; c < END_HANDLER_EVENTS; c++)
	{
		EM->unregisterClient(c, this);
	}
}


void playHandler::HandleEvent(gameEvent* ev)
{
	switch(ev->getEventID())
	{
	case EVENT_GAMELOADING:
		onGameLoad();
		break;
	case EVENT_GAMEPAUSE:
		EM->sendGlobalEvent(GE_STATE_PUSH, new int(STATE_PAUSE));
		break;
	default:
		break;
	}
}

void playHandler::onGameLoad()
{
	//TODO:  Add in a config file that'll grab all the data files that are needed.
	//FL->loadObject("Resource/Triangle.psu");

	//movingObj * testing = (movingObj*)OF->spawn("Triangle");

	movingObj* testObj = new playerObj;

	testObj->setPos(vector3(100,0,0));
	testObj->setAngPos(vector3(0,0,PIHALF));
	testObj->setImgId(viewManager::getInstance()->loadTexture("resource/triangle.bmp", 0xffffffff));
	
	polygon* poly = new polygon;

	objectPoint *points = new objectPoint[3];

	points[0].coords = vector3(5, -27, 0);
	points[1].coords = vector3(29,22,0);
	points[2].coords = vector3(-28,22,0);

	poly->vertecies = points;
	poly->vertexCount = 3;
	poly->maxRadius = 32;

	testObj->setCollisionPoly(poly);

	testObj->setImgCenter(32,32);
	testObj->setDimensions(64,64);

	for(int c = BEGIN_PLAYER_EVENTS; c < END_PLAYER_EVENTS; c++)
		EM->registerClient(c, (playerObj*)testObj);

	((playerObj*)testObj)->setAccStep(0.01f);

	OM->addObj(testObj);
	testObj->release();

	testObj = new movingObj;

	testObj->setPos(vector3(100,200,0));
	testObj->setVel(vector3(0, -200,0));
	testObj->setImgId(viewManager::getInstance()->loadTexture("resource/triangle2.bmp", 0xffffffff));
	
	poly = new polygon;

	poly->vertecies = points;
	poly->vertexCount = 3;
	poly->maxRadius = 32;

	testObj->setCollisionPoly(poly);

	testObj->setImgCenter(32,32);
	testObj->setDimensions(64,64);

	OM->addObj(testObj);

}