#pragma once

class objFactory;

class objFileLoader
{
protected:
	objFactory* OF;
public:
	objFileLoader();
	~objFileLoader();

	//Returns the object factory ID
	char* loadObject(char* filename);
};