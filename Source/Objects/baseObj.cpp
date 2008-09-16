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
#include "objManager.h"
#include "../Engines/CAnimationManager.h"
#include "../Engines/CAnimationEngine.h"

baseObj::baseObj(uint otype, bool movable) : refCount(1), isActive(true), scale(1,1,1), imgId(-1),
type(otype), isMovable(movable), polyEditedThisFrame(false), frameTime(0), collisionPolyID(0),
nAnimNumber(0)
{
	CRITICAL_INIT;

	m_pAM = CAnimationManager::GetInstance();
}

baseObj::baseObj(const baseObj& obj)
{
	*this = obj;
	if(imgId > -1)
		viewManager::getInstance()->acquireTexture(imgId);

	m_pAM->acquireInstance();
}

baseObj::~baseObj(void)
{
	CRITICAL_RELEASE;

	if(imgId != -1)
		viewManager::getInstance()->releaseTexture(imgId);

	m_pAM->releaseInstance();
}

void baseObj::updateWorldMatrix()  //Accounts for world position as well.
{
	matrix combined;
	matrix transform;

	calc::matrixTranslate(transform, vector3((float)-imgCenter.x,(float)-imgCenter.y,0));
	combined *= transform;
	calc::matrixRotationZ(transform, angPos.z);
	combined *= transform;

	calc::matrixScale(transform, scale);
	combined *= transform;
	calc::matrixTranslate(transform, position);
	combined *= transform;

	CRITICAL(worldMatrix = combined);

	//Now modify for camera space
//	worldMatrix *= camera::getInstance()->getViewMatrix();
}

void baseObj::update(float dt)
{
	m_pAM->Update(dt);

	setDimensions(m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->GetWidth(),
				m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->GetHeight());

	pt anchor = *(pt*)(&m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->pAnchor);
	rect draw = *(rect*)&m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->rSource; 

	imgCenter.x = anchor.x - draw.left;
	imgCenter.y = anchor.y - draw.top;

	frameTime = dt;
	polyEditedThisFrame = false;
	updateWorldMatrix();
}
void baseObj::render()
{
	if(imgId == -1)
		return;

	//CRITICAL(viewManager::getInstance()->drawTexture(imgId, NULL, &worldMatrix, &getDrawRect()));

	CRITICAL(m_pAM->Render(nAnimNumber, &worldMatrix));

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
	rect val = *(rect*)&m_pAM->GetEngine(nAnimNumber)->GetCurrentFrame()->rSource;

	val.bottom = val.bottom - val.top;
	val.right = val.right - val.left;
	val.left = val.top = 0;


	val.top += (int)position.y - imgCenter.y;
	val.bottom += (int)position.y - imgCenter.y;
	val.left = (int)(val.left + position.x) - imgCenter.x;
	val.right = (int)(val.right + position.x) - imgCenter.x;

	if(val.left > val.right)
	{
		int holder = val.left;
		val.left = val.right;
		val.right = holder;
	}

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
	const polygon* collisionPoly = objManager::getInstance()->getPoly(collisionPolyID);
	if(instancePoly.vertexCount != collisionPoly->vertexCount)
	{
		instancePoly.vertexCount = collisionPoly->vertexCount;
		
		if(instancePoly.vertecies)
			delete[] instancePoly.vertecies;

		instancePoly.vertecies = new objectPoint[collisionPoly->vertexCount];
		for(int c = 0; c < collisionPoly->vertexCount; c++)
			instancePoly.vertecies[c].mass = collisionPoly->vertecies[c].mass;

		instancePoly.center.mass = collisionPoly->center.mass;
	}

	if(!polyEditedThisFrame)
	{
		for(int c = 0; c < instancePoly.vertexCount; c++)
		{
			if(!calc::isZero(angPos))
				instancePoly.vertecies[c].coords =
					calc::rotatePointAroundOrigin(collisionPoly->vertecies[c].coords, angPos.z)
							+ position;
			else
			{
				instancePoly.vertecies[c].coords = collisionPoly->vertecies[c].coords;
				instancePoly.vertecies[c].coords.x *= scale.x;
				instancePoly.vertecies[c].coords += position;
			}
		}

		instancePoly.center.coords = position;

		polyEditedThisFrame = true;
	}

	return &instancePoly;
}

float baseObj::getMaxRadius() const
{
	return objManager::getInstance()->getPoly(collisionPolyID)->maxRadius;
}

void baseObj::loadAnim(const char* filename)
{
	m_pAM->Load(filename, this);
	//nAnimNumber = 1;
}