#pragma once
#include <iostream>
using std::string;

#include <Windows.h>
#include <vector>
using std::vector;

struct functionInfo
{
	LARGE_INTEGER lnStartTime;
	string szFunctionName;
	vector<LONGLONG> times;
};

class CCodeProfiler
{
private:
	LARGE_INTEGER lnFrequency;
	vector<functionInfo*> vListOfFunctions;


	CCodeProfiler();
	~CCodeProfiler();
	CCodeProfiler(const CCodeProfiler&){}
	CCodeProfiler	& operator=(CCodeProfiler&){}
	
public:

	static CCodeProfiler* GetInstance(void) 
	{
		static CCodeProfiler instance;
		return &instance;
	}

	void Start(string szFunctionName);

	void Stop(string szFunctionName);
};