// Server.h: interface for the Server class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "system/socket.h"	
#include "system/thread.h"
#include "base/logger.h"
#include "message.h"
#include <vector>

using namespace std;
using namespace mr;

namespace mr
{
class Server  
{
public:
	Server(string name);
	virtual ~Server();

	int init(int port, int max_clients=5,bool threaded=true,const char address[]="");	
	void close();


protected:	
	Thread<Server> serverThread;
	vector<Socket*> sockClients;
	vector<Thread<Server> > clients;
	string serverName;
	void* handleClient(void *d=0);
	void* handleConnections(void *d=0);
	virtual string handleRequest(const string& msg)=0;

private:
	Mutex mutex;
	//non copyable, non movable
	Server(const Server&s);
	void operator=(const Server& client);

	bool closeServer;
	Socket socket_server;
	int max_num_clients;
};
};