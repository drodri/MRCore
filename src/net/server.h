#pragma once

#include "system/socket.h"	
#include "system/mutex.h"
#include "system/semaphore.h"
#include "servernode.h"
#include <set>
#include <vector>

using namespace std;
namespace mr
{

class Server  
{
	friend class ServerNode;
public:
	Server(string name);
	virtual ~Server();

	int init(int port,  int max_clients=1,bool threaded=true,const char address[]="");	
	void close();

//Broadcast data to all nodes
	void sendData(const string& msg);
		
protected:
	string serverName;
	Thread<Server> serverThread;
	Mutex mutexNodes;//protect the vector
//	Semaphore semNodes;//avoid accepting more than max
	vector<ServerNode*> nodes;
	//the derived concrete server will have to define these handlers
	virtual void handleData(const string& str)=0;
	virtual string handleRequest(const string& msg)=0;

	///one thread to manage incoming connections
	void* handleConnections(void *d=0);
	//this function will be called by every node when it disconnects
	void disconnect(ServerNode* nod);

private:
	//non copyable, non movable
	Server(const Server&s);
	void operator=(const Server& client);

	bool closeServer;
	Socket socket_server;
	unsigned int max_num_clients;

};

};