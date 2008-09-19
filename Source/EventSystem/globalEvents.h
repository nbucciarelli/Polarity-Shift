//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"globalEvents.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	global events
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

enum globalEvents { GE_NULL = 0xff,

	GE_STATE_PUSH,
	GE_STATE_POP,
	GE_STATE_CHANGETO,
	GE_STATE_CLEARALL,

	GE_GAME_START,
	GE_GAME_END,

GE_MAX
};

enum gameStates { STATE_NULL,

	STATE_MAINMENU,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_OPTIONS,
	STATE_CREDITS,
	STATE_HIGHSCORES,
	STATE_KEYBINDINGS,
	STATE_LEVELSELECT,
	STATE_TALLYSHEET,
	STATE_DEATH,
	STATE_ACHIEVEMENTS,

STATE_MAX
};