#pragma once
#include <vector>
using std::vector;

class baseObj;
union vector3;

typedef baseObj* lpObj;

class objManager
{
protected:
	//The objects in play
	vector<baseObj*> objList;

	//Singleton Protection
	objManager(void);
	objManager(const objManager&);
	objManager& operator=(const objManager&);
	
public:
	~objManager(void);

	static objManager* getInstance();

	void update(float dt);
	void render() const;

	const vector<baseObj*>& getList() const { return objList; }

	//Add an object
	void addObj(baseObj* obj);
	//Remove an object
	void removeObj(baseObj* obj);
	//Remove all objects
	void clear();

	void checkCollisions();
};
