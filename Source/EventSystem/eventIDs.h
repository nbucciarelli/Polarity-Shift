//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"eventIDs.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	List of events
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
enum eventIDs { NULL_EVENT,

	EVENT_ENEMYFIRE,
	EVENT_OBJDIED,

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

//Actor events are global to player & enemies
	BEGIN_ACTOR_EVENTS,

	END_ACTOR_EVENTS,

//Handler events are all routed through the playHandler.
	BEGIN_HANDLER_EVENTS,
	
	EVENT_KILLOBJ,
	EVENT_ACTORDIED,
	EVENT_PLAYERLOADED,
	EVENT_PLAYER_DIED,

	EVENT_WEAPONHIT,
	EVENT_WEAPONMISS,
	EVENT_WEAPONDESTROYED,

	EVENT_GAMEPAUSE,
	EVENT_GAMELEAVE,

	EVENT_NEWLEVEL,
	EVENT_GAMELOADING,
	EVENT_TRAP_ACTIVE,
	EVENT_DIDFALSE,

	END_HANDLER_EVENTS,

//Enemy events are specific to the AI
	BEGIN_AI_EVENTS,

	EVENT_PLAYERLOAD,
	EVENT_ENEMYLOAD,
	EVENT_TURRETLOAD,
	EVENT_POWERUPLOAD,

	END_AI_EVENTS,

//Events for making the Play State be happy.
	BEGIN_PLAY_EVENTS,

	EVENT_LEVELLOADED,
	EVENT_LEVELFINISHED,
	EVENT_DEBUG_SWITCH,

	END_PLAY_EVENTS

};

#define EVENT_PLAYERSPECFIRE EVENT_PLAYERFIRE2