#pragma once

#include "netnode.h"
#include "system/mutex.h"
#include <string>

using namespace std;

namespace mr
{

class Server;

class ServerNode: public NetNode
{
	friend class Server;

protected:
	ServerNode(Server* s, Socket soc){server=s;sock=soc;}
	virtual void handleMsg(const Message& message);
	virtual string handleRequest(const string& msg);
	virtual void handleData(const string& str);
	virtual void onDisconnect();

protected:
	Server* server;
};

};