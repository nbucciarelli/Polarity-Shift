#pragma once

#include "datatypes.h"

enum objTypes { OBJ_DEFAULT, OBJ_MOVING,
				OBJ_MAX };
enum facing { FACE_LEFT = 1, FACE_RIGHT = -1 };

class baseObj
{
protected:
	//Location and movement data, 3d vectors
	//Note that the angular data is really just a collection of information for
	//radians data of x,y,z angular positioning, not a true vector.
	vector3 position, angPos, scale, nullVector;

	//object's local space transformation matrix
	matrix worldMatrix;
	//(Technically, the vec3 position is redundant, but oh well.)
	
	//Simple method for critical sections.
	//If the world matrix is being updated or used in render, this will be true.
	volatile bool locked;

	uint type;

	int imgId;

	uint refCount;
	bool isActive;

	//Size of rendered portion
	pt dimension;
	//the rotation location within the dimensions, with imgPos translated to 0,0.
	pt imgCenter;
	//the full size of the image
	pt imgSize;
	//the coordinates of the top left corner of rendered portion
	pt imgPos;

	rect getDrawRect() const;
public:
	baseObj(uint otype = OBJ_DEFAULT);
	baseObj(const baseObj&);
	virtual ~baseObj(void);

	//Standard game loop functions
	virtual void update(float dt);
	virtual void render();

	virtual bool checkCollision(baseObj* obj, rect* over = NULL);

	//Ref counting functions
	void acquire() { refCount++; }
	void release();

	//world matrix calculation
	void updateWorldMatrix();

	//Accessors
	const pt getDimensions() const { return dimension; }
	const pt getImgCenter() const { return imgCenter; }

	//This exists so that some of the "standing" functions work correctly.
	//(hax.)
	virtual const vector3& getVelocity() const { return nullVector; }
	const vector3& getPosition() const { return position; }
	const vector3& getAngPos() const { return angPos; }
	const matrix& getWorldMatrix() const { return worldMatrix; }
	const vector3& getScale() const { return scale; }
	int getImgId() const { return imgId; }
	int getType() const { return type; }

	int getFacing() const { return (int)scale.x; }
	rect getCollisionRect() const;

	//Set Mutators: Change variable to given data
	void setDimensions(int x, int y) { dimension = pt(x,y); }
	void setImgCenter(int x, int y) { imgCenter = pt(x,y); }
	void setPos(const vector3& pos) { position = pos; }
	void setAngPos(const vector3& aPos) { angPos = aPos; }
	void setScale(const vector3& scl) { scale = scl; }
	void setImgId(int id);

	void setFacing(int face) { scale.x = (float)face; }

	//Mod Mutators: Add given data to variable
	void modPos(const vector3& change) { position += change; }
	void modAngPos(const vector3& change) { angPos += change; }
};
