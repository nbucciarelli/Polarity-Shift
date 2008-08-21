#pragma once

/*
	View Manager:  Stores & handles drawing of all sprites and meshes.
	
	Singleton.
*/

#include <vector>
using std::vector;

#include "datatypes.h"

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

	void drawTexture(int id, const vector3* pos, const matrix * transform = NULL, const rect* section = NULL,
		const vector3* center = NULL, uint color = 0xffffffff);

	void acquireTexture(int textureID) { textureList[textureID]->ref++; }
	void releaseTexture(int textureID);
	void releaseMesh(int meshID);

	void releaseAllTextures();
	void releaseAllMeshes();

	pt getTextureDimensions(int id) const;
};
