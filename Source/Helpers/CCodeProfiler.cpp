#include "CCodeProfiler.h"
#include <string>
#include <fstream>

void CCodeProfiler::Start( string szFunctionName )
{

	int index = -1;
	for (int i = 0; i < (int)vListOfFunctions.size(); ++i)
	{
		if (szFunctionName == vListOfFunctions[i]->szFunctionName)
		{
			index = i;
		}
	}

	if (index == -1)
	{
		functionInfo* fi = new functionInfo;
	
		fi->szFunctionName = szFunctionName;

		vListOfFunctions.push_back(fi);

		index = (int)vListOfFunctions.size() - 1;
	}

	QueryPerformanceCounter(&vListOfFunctions[index]->lnStartTime);

}

void CCodeProfiler::Stop( string szFunctionName )
{
	for (int i = 0; i < (int)vListOfFunctions.size(); ++i)
	{
		if (szFunctionName == vListOfFunctions[i]->szFunctionName)
		{
			LARGE_INTEGER tmpTimer;
	
			QueryPerformanceCounter(&tmpTimer);
	
			vListOfFunctions[i]->times.push_back(tmpTimer.QuadPart - vListOfFunctions[i]->lnStartTime.QuadPart);
		}
	}
}

CCodeProfiler::CCodeProfiler()
{
	QueryPerformanceFrequency(&lnFrequency);
}

CCodeProfiler::~CCodeProfiler()
{
	for (int i = 0; i < (int)vListOfFunctions.size(); ++i)
	{
		string fileName = "Resource//CodeProfiler//";
		fileName += vListOfFunctions[i]->szFunctionName;
		fileName += ".txt";
		std::ofstream fout(fileName.c_str());

		if (fout.is_open())
		{
			double total = 0.0f;
			int max = 0;
			for (int j = 0; j < (int)vListOfFunctions[i]->times.size(); ++j)
			{
				total += vListOfFunctions[i]->times[j];
				if (max < vListOfFunctions[i]->times[j])
					max = (int)vListOfFunctions[i]->times[j];
			}
			
			total = vListOfFunctions[i]->times.size()/total;
			fout << "Avg Time (In Seconds): " << (total/lnFrequency.QuadPart*1000) << "\n";
			fout << "Max Time (In Seconds): " << ((double)max/lnFrequency.QuadPart*1000) << "\n\n\n";

			for (int j = 0; j < (int)vListOfFunctions[i]->times.size(); ++j)
				fout << ((double)vListOfFunctions[i]->times[j]/lnFrequency.QuadPart*1000) << '\n';
		}	
	}

	for (int i = 0; i < (int)vListOfFunctions.size(); ++i)
		delete vListOfFunctions[i];
}