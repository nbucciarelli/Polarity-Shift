#pragma once

class baseObj;
class movingObj;
class playerObj;

class objFactory;

class objFileLoader
{
protected:
	objFactory* OF;
public:
	objFileLoader();
	~objFileLoader();
	void loadObject(char* filename);
};