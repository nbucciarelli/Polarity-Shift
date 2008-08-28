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

	EVENT_PLAYERFIRE,
	EVENT_PLAYERSPECFIRE,

	EVENT_PLAYERKILLED,

	END_PLAYER_EVENTS,

//Handler events are all routed through the "handler" singleton.
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

};