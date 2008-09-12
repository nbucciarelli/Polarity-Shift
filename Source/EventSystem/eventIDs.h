//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"eventIDs.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	List of events
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
enum eventIDs { NULL_EVENT,

	EVENT_ENEMYFIRE,

//Player events are specific to the...player, amazingly.
	BEGIN_PLAYER_EVENTS,

	EVENT_PLAYERGOLEFT,
	EVENT_PLAYERGORIGHT,
	EVENT_PLAYERSTOP,

	EVENT_PLAYERJUMP,
	EVENT_PLAYERJUMPSTOP,

	EVENT_PLAYERFIRE,
	EVENT_PLAYERFIRE2,
	EVENT_PLAYERFIRE3,
	EVENT_PLAYERCEASEFIRE,

	EVENT_PLAYERKILLED,

	END_PLAYER_EVENTS,

//Handler events are all routed through the playHandler.
	BEGIN_HANDLER_EVENTS,
	
	EVENT_ACTORDIED,
	EVENT_PLAYERLOADED,

	EVENT_WEAPONHIT,
	EVENT_WEAPONMISS,
	EVENT_WEAPONDESTROYED,

	EVENT_GAMEPAUSE,
	EVENT_GAMELEAVE,

	EVENT_NEWLEVEL,
	EVENT_GAMELOADING,

	END_HANDLER_EVENTS,

//Enemy events are specific to the AI
	BEGIN_AI_EVENTS,

	EVENT_ENEMYLOAD,
	EVENT_PLAYERLOAD,

	END_AI_EVENTS,

//Events for making the Play State be happy.
	BEGIN_PLAY_EVENTS,

	EVENT_LEVELLOADED,
	EVENT_LEVELFINISHED,

	END_PLAY_EVENTS

};

#define EVENT_PLAYERSPECFIRE EVENT_PLAYERFIRE2