#include "messageManager.h"
#include "message.h"

#pragma region constructor / destructor
messageManager::messageManager(void)
{
}

messageManager::~messageManager(void)
{
}
#pragma endregion

#pragma region singleton
messageManager* messageManager::getInstance()
{
	static messageManager herald;
	return &herald;
}
#pragma endregion

#pragma region init/shutdown
void messageManager::initialize(MessageProc _msgProc)
{
	msgProc = _msgProc;
}

void messageManager::shutdown()
{
	clearMessages();
	msgProc = 0;
}
#pragma endregion

#pragma region message_handling
void messageManager::sendMsg(message* msg)
{
	if (!msg) //OSHI- protection
		return;

	//Throw that beast on the pile
	outbox.push(msg);
}

void messageManager::processMessages()
{
	if(!msgProc) //OSHI- protection
		return;
	
	while(!outbox.empty())
	{
		msgProc(outbox.front());
		delete outbox.front();
		outbox.pop();
	}
}

void messageManager::clearMessages()
{
	while(!outbox.empty())
	{
		delete outbox.front();
		outbox.pop();
	}
}
#pragma endregion