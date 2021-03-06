//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"messageManager.h"
//	Author:		Scott Smallback (SS)
//	Purpose:	Manages the messages
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <queue>
using std::queue;

class message;

typedef void (*MessageProc)(message*);

class messageManager
{
protected:
	queue<message*> outbox;
	MessageProc msgProc;

	messageManager(void);
	messageManager(const messageManager&);
	messageManager& operator=(const messageManager&);
public:
	~messageManager(void);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"getInstance"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Gets the instance of the singleton
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static messageManager* getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"initialize"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Initializes the message system
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void initialize(MessageProc _msgProc);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"shutdown"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Shuts down the message system
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void shutdown();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"sendMsg"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Sends a message
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void sendMsg(message* msg);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"processMessages"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Handles the messages waiting
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void processMessages();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"clearMessages"
	//	Last Modified:	August 25, 2008
	//	Purpose:		Dumps the message queue
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void clearMessages();
};
