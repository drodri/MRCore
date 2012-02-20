#pragma once

#include "system/socket.h"	
#include "system/mutex.h"
#include "system/thread.h"
#include "message.h"
#include <string>

using namespace std;

namespace mr
{
enum {COMM_OK, COMM_ERROR, COMM_TIMEOUT, COMM_ERROR_MSG};

class NetNode 
{
public:
	int isConnected(){return sock.IsConnected();}
	int isValid(){return sock.IsValid();}

protected:	
	NetNode ();
	virtual ~NetNode ();
	
	//asyncr message, streaming. The other node will receive in its handleData()
	int sendData(const string& str);

	//close socket and waits (join) for thread to finish
	void close();

	///one thread per node, this thread will be launched by derived classes
	void startThread();
	void* receiveMessages(void* d=0);
	//delegates the message handling to derived class
	virtual void handleMsg(const Message& msg)=0;
	virtual void handleData(const string& str)=0;
	virtual void onDisconnect(){};
	
	Socket sock;
	bool closeNode;
	Thread<NetNode> receiveThread;
};

};