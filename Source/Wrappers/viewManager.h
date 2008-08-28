#pragma once

/*
	View Manager:  Stores & handles drawing of all sprites and meshes.
	
	Singleton.
*/

#include <vector>
using std::vector;

#include "..\Helpers\datatypes.h"

class renderer;
struct meshData;

typedef unsigned int uint;

class viewManager
{
protected:
	// All the data we need to contain a texture.

	vector<textureData*> textureList;
	vector<meshData*> meshList;

	renderer* theRenderer;

	viewManager(void);
	viewManager(const viewManager&);
	viewManager& operator=(const viewManager&);
public:
	~viewManager(void);

	static viewManager* getInstance();

	void initialize(renderer*);
	void shutdown();

	//Returns texture ID
	int loadTexture(const char filename[], uint colorKey = 0);
	int loadMesh(char filename[]);

	//drawTexture does NOT do computations for scaling, rotation, anything.
	//Everything for that should be done with a transformation matrix.
	//(see the "calc" class in physics.h)
	//The vec3 position will offset the drawing from where the transformation
	//matrix designates.  Can be null in all circumstances.
	void drawTexture(int id, const vector3* pos = NULL, const matrix * transform = NULL, const rect* section = NULL,
		const vector3* center = NULL, uint color = 0xffffffff);

	//If you know what the ID is, it can be acquired.  Basically, for copying objects.
	void acquireTexture(int textureID) {
	//	if(textureID > 0 && textureID < (int)textureList.size())
			textureList[textureID]->ref++;
	}
	void releaseTexture(int textureID);
	void releaseMesh(int meshID);

	void releaseAllTextures();
	void releaseAllMeshes();

	pt getTextureDimensions(int id) const;

	unsigned int color_argb(char a, char r, char g, char b);
};
