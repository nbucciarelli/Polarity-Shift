#pragma once

#include <vector>
using std::vector;

typedef int IDType;

class baseObj;

struct creationEntry
{
	typedef baseObj* (*createFunc)(baseObj*);

	IDType id;
	baseObj* _template;
	createFunc creator;
};

class objFactory
{
protected:

	vector<creationEntry> creationList;

	objFactory(void);
	objFactory(const objFactory&);
	objFactory& operator=(const objFactory&);

	template <typename classType>
	static baseObj* create(baseObj* src = NULL)
	{ 
		if(src)
			return new classType( *((classType*)src) );
		else
			return new classType;
	}

public:
	~objFactory(void);

	static objFactory* getInstance();

	template <typename classType>
	void registerClass(IDType id, baseObj* _template)
	{
		creationEntry objEntry;

		objEntry.id = id;
		objEntry.creator = create<classType>;
		objEntry._template = _template;

		//	Add this information into my database.
		creationList.push_back(objEntry);
	}

	void unregisterClass(IDType id);
	void unregisterAll();

	baseObj* spawn(IDType id);
};
