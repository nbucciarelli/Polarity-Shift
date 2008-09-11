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
#include "../Wrappers/Mouse.h"
#include "../Engines/CTileEngine.h"

#include "../Objects/weapon.h"
#include "../Objects/playerObj.h"
#include "../Objects/enemyObj.h"

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

	mouse* mousy = mouse::getInstance();
	mousy->initialize();

	mousy->setImgID(viewManager::getInstance()->loadTexture("resource/images/PS_cursor.tga", 0xffff00ff));
	mousy->setPos(vector3(512,320,0));
	mousy->setClickPos(pt(32,32));

	movingObj* testObj = (movingObj*)OF->spawn(
		FL->loadObject("Resource/PS_ironman.psu"));

	testObj->setPos(vector3(CTileEngine::GetInstance()->GetPlayerSpawn().x,CTileEngine::GetInstance()->GetPlayerSpawn().y,0));
	//testObj->setPos(vector3(200,0,0));
	for(int c = BEGIN_PLAYER_EVENTS; c < END_PLAYER_EVENTS; c++)
		EM->registerClient(c, (playerObj*)testObj);

	((playerObj*)testObj)->setAccStep(1);
	((playerObj*)testObj)->setWeapon(WEAPON_MAGNET);
	weapon* weapy = ((playerObj*)testObj)->getWeapon();

	weapy->setPower(200);
	weapy->setRange(800);
	EM->sendEvent(EVENT_PLAYERLOAD, testObj);

	OM->addObj(testObj);
	testObj->release();
	//

	FL->loadObject("Resource/PS_triangle.psu");
	testObj = (movingObj*)OF->spawn("Gadzooks");

	testObj->setPos(vector3(100,200,0));
	//testObj->setAngPos(vector3(0,0,PI));
	testObj->setVel(vector3(0, -200,0));

	testObj->setImgCenter(32,32);
	testObj->setDimensions(64,64);

	OM->addObj(testObj);
	testObj->release();

	
	testObj = new enemyObj;

	testObj->setPos(vector3(700,200,0));
	//testObj->setAngPos(vector3(0,0,PI));
	testObj->setVel(vector3(0, -200,0));
	testObj->setImgId(viewManager::getInstance()->loadTexture("resource/PS_triangle2.bmp", 0xffffffff));
	
	polygon* poly = new polygon;
	objectPoint *points = new objectPoint[3];

	points[2].coords = vector3(5, -27, 0);
	points[1].coords = vector3(29,22,0);
	points[0].coords = vector3(-28,22,0);

	poly->vertecies = points;
	poly->vertexCount = 3;
	poly->maxRadius = 32;

	testObj->setCollisionPoly(poly);

	testObj->setImgCenter(32,32);
	testObj->setDimensions(64,64);


	EM->sendEvent(EVENT_ENEMYLOAD, testObj);
	OM->addObj(testObj);
	testObj->release();

	//Let play state begin.
	EM->sendEvent(EVENT_LEVELLOADED);
}