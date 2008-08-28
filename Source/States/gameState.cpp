#include "gameState.h"

void gameState::enter(void)
{
	entered = true;
}

void gameState::exit()
{
	entered = false;
}