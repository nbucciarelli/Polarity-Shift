////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “baseObj.cpp”
// Author: Scott Smallback (SS)
// Purpose: This is the code file for the base object
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "baseObj.h"
#include "..\Helpers\physics.h"
#include "..\Wrappers\viewManager.h"
#include "camera.h"
#include "../Wrappers/dxRenderer.h"
#include "../Helpers/criticalSection.h"

baseObj::baseObj(uint otype, bool movable) : refCount(1), isActive(true), scale(1,1,1), imgId(-1),
type(otype), isMovable(movable)
{
	CRITICAL_INIT;
}

baseObj::baseObj(const baseObj& obj)
{
	*this = obj;
	if(imgId > -1)
		viewManager::getInstance()->acquireTexture(imgId);
}

baseObj::~baseObj(void)
{
	CRITICAL_RELEASE;

	if(imgId != -1)
		viewManager::getInstance()->releaseTexture(imgId);

	if(instancePoly.vertecies)
		delete[] instancePoly.vertecies;
}

void baseObj::updateWorldMatrix()  //Accounts for world position as well.
{
	matrix combined;
	matrix transform;

	calc::matrixTranslate(transform, vector3((float)-imgCenter.x,(float)-imgCenter.y,0));
	combined *= transform;
	calc::matrixRotationZ(transform, angPos.z);
	combined *= transform;
	//calc::matrixTranslate(transform, vector3((float)imgCenter.x, (float)imgCenter.y, 0));
	//combined *= transform;

	calc::matrixScale(transform, scale);
	combined *= transform;
	calc::matrixTranslate(transform, position);
	combined *= transform;

	CRITICAL(worldMatrix = *((matrix*)&combined));

	//Now modify for camera space
//	worldMatrix *= camera::getInstance()->getViewMatrix();
}

void baseObj::update(float dt)
{
	frameTime = dt;
	updateWorldMatrix();
}
void baseObj::render()
{
//	WAIT;

//	locked = true;
//	viewManager::getInstance()->drawTexture(imgId, NULL, &worldMatrix, &getDrawRect());
//	locked = false;

	if(imgId == -1)
		return;

	CRITICAL(viewManager::getInstance()->drawTexture(imgId, NULL, &worldMatrix, &getDrawRect()));

}
rect baseObj::getDrawRect() const
{
	rect val;
	int x,y;
	x = dimension.x / 2;
	y = dimension.y / 2;

	val.top = imgCenter.y - y;
	val.bottom = imgCenter.y + y;
	val.left = imgCenter.x - x;
	val.right = imgCenter.x + x;

	return val;
}
rect baseObj::getCollisionRect() const
{
	rect val;
	int x,y,xOffset,yOffset;
	x = dimension.x / 2;
	y = dimension.y / 2;
	xOffset = (imgSize.x - dimension.x) / 2;
	yOffset = (imgSize.y - dimension.y) / 2;
	val.top = (int)position.y - y - yOffset;
	val.bottom = (int)position.y + y - yOffset;
	val.left = (int)(position.x - x - scale.x * xOffset);
	val.right = (int)(position.x + x - scale.x * xOffset);

	return val;
}
bool baseObj::checkCollision(baseObj* obj, polyCollision* result)
{
	return false;
}

void baseObj::release()
{
	refCount--;

	if(refCount < 1)
		delete this;
}

void baseObj::setImgId(int id)
{
	viewManager* vm = viewManager::getInstance();
	if(imgId != -1)
		vm->releaseTexture(imgId);

	imgId = id;
	vm->acquireTexture(imgId);

	imgSize = vm->getTextureDimensions(imgId);
}

const polygon* baseObj::getCollisionPoly()
{
	if(instancePoly.vertexCount != collisionPoly->vertexCount)
	{
		instancePoly.vertexCount = collisionPoly->vertexCount;
		
		if(instancePoly.vertecies)
			delete instancePoly.vertecies;

		instancePoly.vertecies = new objectPoint[collisionPoly->vertexCount];
		for(int c = 0; c < collisionPoly->vertexCount; c++)
			instancePoly.vertecies[c].mass = collisionPoly->vertecies[c].mass;

		instancePoly.center.mass = collisionPoly->center.mass;
	}


	for(int c = 0; c < instancePoly.vertexCount; c++)
	{
		instancePoly.vertecies[c].coords = collisionPoly->vertecies[c].coords + position;
		instancePoly.vertecies[c].coords.x -= imgCenter.x;

	}

	instancePoly.center.coords = position;

	return &instancePoly;
}