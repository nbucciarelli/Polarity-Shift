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
	const char* loadObject(char* filename);
};