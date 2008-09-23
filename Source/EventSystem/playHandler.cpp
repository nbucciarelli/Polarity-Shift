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
#include "..\States\GameStates\gamePlayState.h"
#include "../States/GameStates/levelChooseState.h"

#include "../Objects/weapon.h"
#include "../Objects/playerObj.h"
#include "../Objects/enemyObj.h"
#include "../Objects/CPowerUp.h"

#include "..\Wrappers\CSGD_FModManager.h"
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
	static bool did = false;
	switch(ev->getEventID())
	{
	case EVENT_GAMELOADING:
		did = false;
		onGameLoad();
		break;
	case EVENT_GAMEPAUSE:
		EM->sendGlobalEvent(GE_STATE_PUSH, new int(STATE_PAUSE));
		break;
	case EVENT_KILLOBJ:
		OM->removeObj((baseObj*)ev->getData());
		break;
	case EVENT_ACTORDIED:
		killActor((baseObj*)ev->getData());
		break;
	case EVENT_PLAYER_DIED:
		EM->sendGlobalEvent(GE_STATE_CHANGETO, new int(STATE_DEATH));
		break;
	case EVENT_DIDFALSE:
		did = false;
		break;
	case EVENT_TRAP_ACTIVE:
		if(did)
			break;


		enemyObj* newEnemyObj = (enemyObj*)OF->spawn("spider");

		newEnemyObj->setPos(vector3(CTileEngine::GetInstance()->GetTraps()[0].x,CTileEngine::GetInstance()->GetTraps()[0].y,0));
		EM->sendEvent(EVENT_ENEMYLOAD, newEnemyObj);
		OM->addObj(newEnemyObj);
		newEnemyObj->release();
		did = true;
		//EM->unregisterClient(EVENT_TRAP_ACTIVE, this);

		break;



	//default:
	//	break;
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

	char* obid = FL->loadObject("Resource/PS_riley.psu");
	movingObj* testObj = (movingObj*)OF->spawn(obid);

	delete[] obid;

	testObj->setPos(
		vector3((float)CTileEngine::GetInstance()->GetPlayerSpawn().x,
			(float)CTileEngine::GetInstance()->GetPlayerSpawn().y,0));
	//testObj->setPos(vector3(200,0,0));


	//((playerObj*)testObj)->setAccStep(1);
	//((playerObj*)testObj)->setWeapon(WEAPON_MAGNET);
	//weapon* weapy = ((playerObj*)testObj)->getWeapon();

	//weapy->setPower(200);
	//weapy->setRange(800);
	//EM->sendEvent(EVENT_PLAYERLOAD, testObj);

	for(int c = BEGIN_PLAYER_EVENTS; c < END_PLAYER_EVENTS; c++)
		eventManager::getInstance()->registerClient(c, (playerObj*)testObj);
	eventManager::getInstance()->sendEvent(EVENT_PLAYERLOAD, testObj);

	OM->addObj(testObj);
	testObj->release();

		obid = FL->loadObject("Resource/PS_boxy.psu");

	for(int i = 0; i <CTileEngine::GetInstance()->GetCubes().size(); i ++)
	{

		//
		testObj = (movingObj*)OF->spawn(obid);

		testObj->setPos(vector3(CTileEngine::GetInstance()->GetCubes()[i].x,CTileEngine::GetInstance()->GetCubes()[i].y,0));
		//testObj->setAngPos(vector3(0,0,PI));
		//testObj->setVel(vector3(0, -200,0));

		//testObj->setImgCenter(32,32);
		//testObj->setDimensions(64,64);

		OM->addObj(testObj);
		testObj->release();
	}
	delete[] obid;

	obid = FL->loadObject("Resource/PS_banana.psu");
	enemyObj* testObj3;
	for(int i = 0; i <CTileEngine::GetInstance()->GetBonuses().size(); i++)
	{
		testObj3 = (enemyObj*)OF->spawn(obid);
		testObj3->setEnemyType(ET_SCORE);
		testObj3->setPos(vector3(CTileEngine::GetInstance()->GetBonuses()[i].x,CTileEngine::GetInstance()->GetBonuses()[i].y,0));
		EM->sendEvent(EVENT_POWERUPLOAD, testObj3);
		OM->addObj(testObj3);
		testObj3->release();
	}
	delete[] obid;
	
	obid = FL->loadObject("Resource/PS_light.psu");
	for(int i = 0; i <CTileEngine::GetInstance()->GetInv().size(); i++)
	{
		testObj3 = (enemyObj*)OF->spawn(obid);
		testObj3->setEnemyType(ET_INV);
		testObj3->setPos(vector3(CTileEngine::GetInstance()->GetInv()[i].x,CTileEngine::GetInstance()->GetInv()[i].y,0));
		EM->sendEvent(EVENT_POWERUPLOAD, testObj3);
		OM->addObj(testObj3);
		testObj3->release();
	}
	delete[] obid;

	obid = FL->loadObject("Resource/PS_spider.psu");
	for(int i = 0; i <CTileEngine::GetInstance()->GetEnemies().size(); i ++)
	{
		testObj = (enemyObj*)OF->spawn(obid);

		testObj->setPos(vector3(CTileEngine::GetInstance()->GetEnemies()[i].x,CTileEngine::GetInstance()->GetEnemies()[i].y,0));


		EM->sendEvent(EVENT_ENEMYLOAD, testObj);
		OM->addObj(testObj);
		testObj->release();
	}
	delete[] obid;

	obid = FL->loadObject("Resource/PS_turret.psu");
	for(int i = 0; i <CTileEngine::GetInstance()->GetTurrets().size(); i ++)
	{
		testObj = (enemyObj*)OF->spawn(obid);

		testObj->setPos(vector3(CTileEngine::GetInstance()->GetTurrets()[i].x,CTileEngine::GetInstance()->GetTurrets()[i].y,0));


		EM->sendEvent(EVENT_ENEMYLOAD, testObj);
		OM->addObj(testObj);
		testObj->release();
	}
	delete[] obid;

	if(levelChooseState::getInstance()->GetPlayLevel() == 10)
	{
	obid = FL->loadObject("Resource/PS_Boss.psu");

	enemyObj* testObj2 = (enemyObj*)OF->spawn(obid);
	testObj2->setEnemyType(ET_BOSS);
	testObj2->setPos(vector3(900,100,0));
	EM->sendEvent(EVENT_ENEMYLOAD, testObj2);
	OM->addObj(testObj2);
	testObj2->release();
	delete[] obid;
	}


	//Let play state begin.
	EM->sendEvent(EVENT_LEVELLOADED);
}

void playHandler::killActor(baseObj* obj)
{
	gamePlayState::getInstance()->SetExpX(obj->GetPosX());
	gamePlayState::getInstance()->SetExpY(obj->GetPosY());
	gamePlayState::getInstance()->SetIsExploding(true);
	obj->setIsActive(false);
	//EM->unregisterClient((movingObj*)obj);
	//OM->removeObj(obj);
}