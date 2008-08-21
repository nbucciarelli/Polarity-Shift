#include <windows.h>

#define VLD_AGGREGATE_DUPLICATES
#define VLD_MAX_DATA_DUMP 0
#include <vld.h>

#include "game.h"

int WINAPI WinMain( HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	game* theGame = game::GetInstance();

	theGame->Initialize(hInstance);
	theGame->Run();
	theGame->Shutdown();

	return 0;
}