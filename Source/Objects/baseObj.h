#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “baseObj.h”
// Author: Scott Smallback (SS)
// Purpose: This is the header file for any object that will be created
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\Helpers\datatypes.h"

#include "../Helpers/criticalSectionMacros.h"

enum objTypes { OBJ_DEFAULT, OBJ_MOVING, OBJ_PLAYER, OBJ_ENEMY,
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
	
	//critical section stuff
	CRITICAL_VARS;

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

	bool isMovable;
	const polygon* collisionPoly;
	polygon instancePoly;
	float frameTime;

	rect getDrawRect() const;
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Constructor”
	// Last Modified: August 27, 2008
	// Purpose: This is the constructor for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	baseObj(uint otype = OBJ_DEFAULT, bool movable = false);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Copy Constructor”
	// Last Modified: August 27, 2008
	// Purpose: This is the copy constructor for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	baseObj(const baseObj&);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Deconstructor”
	// Last Modified: August 27, 2008
	// Purpose: This is the deconstructor for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~baseObj(void);

	//Standard game loop functions
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Update”
	// Last Modified: August 27, 2008
	// Purpose: This is the update for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void update(float dt);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Render”
	// Last Modified: August 27, 2008
	// Purpose: This is the render for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void render();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “checkCollision”
	// Last Modified: August 27, 2008
	// Purpose: This is the check collision for the base object touching other objects
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool checkCollision(baseObj* obj, polyCollision* result = NULL);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “checkTerrainCollision”
	// Last Modified: August 27, 2008
	// Purpose: This is the check terrain collision for the base object touching terrain
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool checkTerrainCollision(rect& colBounds) { return false; }

	//Ref counting functions
	void acquire() { refCount++; }
	void release();

	//world matrix calculation
	void updateWorldMatrix();

	const polygon* getCollisionPoly();
	bool polyEditedThisFrame;
	bool IsMovable() const { return isMovable; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Accessors”
	// Last Modified: August 27, 2008
	// Purpose: These are the accessors for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//This exists so that some of the "standing" functions work correctly.
	//(hax.)
	const pt getDimensions() const { return dimension; }
	const pt getImgCenter() const { return imgCenter; }
	const float getMaxRadius() const { return collisionPoly->maxRadius; }
	virtual const vector3& getVelocity() const { return nullVector; }
	const vector3& getPosition() const { return position; }
	const vector3& getAngPos() const { return angPos; }
	const matrix& getWorldMatrix() const { return worldMatrix; }
	const vector3& getScale() const { return scale; }
	int getImgId() const { return imgId; }
	int getType() const { return type; }
	int getFacing() const { return (int)scale.x; }
	virtual rect getCollisionRect() const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Mutators”
	// Last Modified: August 27, 2008
	// Purpose: These are the mutators for the base object
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Set Mutators: Change variable to given data
	void setDimensions(int x, int y) { dimension = pt(x,y); }
	void setImgCenter(int x, int y) { imgCenter = pt(x,y); }
	void setPos(const vector3& pos) { position = pos; }
	void setAngPos(const vector3& aPos) { angPos = aPos; }
	void setScale(const vector3& scl) { scale = scl; }
	void setImgId(int id);
	void setCollisionPoly(polygon* poly) { collisionPoly = poly; }
	void setFacing(int face) { scale.x = (float)face; }

	//Mod Mutators: Add given data to variable
	void modPos(const vector3& change) { position += change; }
	void modAngPos(const vector3& change) { angPos += change; }
};
