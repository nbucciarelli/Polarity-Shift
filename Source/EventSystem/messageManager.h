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

	static messageManager* getInstance();

	void initialize(MessageProc _msgProc);
	void shutdown();

	void sendMsg(message* msg);

	// Handle messages waiting
	void processMessages();

	//dump message queue
	void clearMessages();
};
