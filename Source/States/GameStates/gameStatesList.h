#pragma once

/*
	GameStatesList.h : Header includes for all game states.

	In most cases, this does not need to be included, but for things such as the
	Global Events Handler, it is convenient to use this, rather than constantly
	adding the lengthy relative paths into game states.

	When creating a new game state, add their header here, and an identifier
	in the global events header's enum.
*/

#include "mainMenuState.h"
#include "gamePlayState.h"
#include "pauseState.h"
#include "optionsState.h"
#include "CHighScoresState.h"
#include "CCreditsState.h"
#include "CKeyState.h"
#include "levelChooseState.h"