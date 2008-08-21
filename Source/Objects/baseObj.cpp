#include <windows.h>
#include "baseObj.h"
#include "..\Helpers\physics.h"
#include "..\Wrappers\viewManager.h"
#include <d3dx9.h>
#include "camera.h"

#pragma comment(lib, "d3dx9.lib")

#define WAIT while(locked) {}

baseObj::baseObj(uint otype) : refCount(1), isActive(true), scale(1,1,1), imgId(-1),
type(otype), locked(false)
{}

baseObj::baseObj(const baseObj& obj)
{
	*this = obj;
	if(-1 != imgId)
		viewManager::getInstance()->acquireTexture(imgId);
}

baseObj::~baseObj(void)
{
	if(imgId != -1)
		viewManager::getInstance()->releaseTexture(imgId);
}

void baseObj::updateWorldMatrix()  //Accounts for world position as well.
{
//	calc::matrixTranslate(worldMatrix, vector3((float)-imgCenter.x,(float)-imgCenter.y,0));
	
//	matrix transform;
//	calc::matrixRotation(worldMatrix, angPos);
//	worldMatrix *= transform;

//	calc::matrixTranslate(transform, vector3((float)imgCenter.x, (float)imgCenter.y, 0));
//	worldMatrix *= transform;

//	calc::matrixScale(transform, scale);
//	worldMatrix *= transform;

//	calc::matrixTranslate(transform, position);
//	worldMatrix *= transform;

	D3DXMATRIX transform;
	D3DXMATRIX combined;

	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&combined);

	// Rotate the sprite.
	D3DXMatrixTranslation(&transform, (float)-imgCenter.x, (float)-imgCenter.y, 0.0f);
	combined *= transform;
	D3DXMatrixRotationZ(&transform, angPos.z);
	combined *= transform;

	// Scale the sprite.
	D3DXMatrixScaling(&transform, scale.x, scale.y, 1.0f);
	combined *= transform;

	// Translate the sprite
	D3DXMatrixTranslation(&transform, position.x, position.y, 0.0f);
	combined *= transform;

	WAIT;

	locked = true;
	worldMatrix = *reinterpret_cast<matrix*>(&combined);
	locked = false;
	//memcpy((void*)&worldMatrix, &combined, sizeof(matrix));

	//Now modify for camera space
//	worldMatrix *= camera::getInstance()->getViewMatrix();
}

void baseObj::update(float dt)
{
	updateWorldMatrix();
}
void baseObj::render()
{
	WAIT;

	locked = true;
	viewManager::getInstance()->drawTexture(imgId, NULL, &worldMatrix, &getDrawRect());
	locked = false;
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
bool baseObj::checkCollision(baseObj* obj, rect* over)
{
	if(obj != this)
	{
		if(!over)
		{
			RECT overlap;
			if(IntersectRect(&overlap,
				reinterpret_cast<const RECT*>(&getCollisionRect()),
				reinterpret_cast<const RECT*>(&obj->getCollisionRect()))
				)
				return true;
		}
		else
		{
			if(IntersectRect(reinterpret_cast<RECT*>(over),
				reinterpret_cast<const RECT*>(&getCollisionRect()),
				reinterpret_cast<const RECT*>(&obj->getCollisionRect()))
				)
				return true;
		}
	}
	
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