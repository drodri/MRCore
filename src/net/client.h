// HttpClient.h: interface for the HttpClient class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "system/socket.h"
#include "system/thread.h"
#include "message.h"

#include <string>
using namespace mr;

enum {COMM_OK, COMM_ERROR, COMM_TIMEOUT, COMM_ERROR_MSG};

namespace mr
{
class Client 
{
public:
	Client();
	virtual ~Client();

//warppers of socket connection
	int connect(const char address[]="127.0.0.1",int port=80,bool wantReconnect=true);	
	void close();

	string getHost(){return host;}
	string getAddress(){return host_addr;}
	int getPort(){return host_port;}

	int sendRequest(string request,string& response,int timeout=50);

	int isConnected(){return sock.IsConnected();}
	int isValid(){return sock.IsValid();}
	

protected:
	void* ReconnectLoop(void* d);///Thread to automatically reconnect
	string WaitResponse(int timeout=50);

	Socket sock;
	Mutex mutex;//so the http_client is thread_safe. TODO: SmartPointer for move semantics. Check copyable and movable concepts on HTTPClient

	string host;
	string host_addr;
	int host_port;
	bool wantReconnect;///Default: false. If we connected and the connection is lost. Get and Post try to reconnect, which can delay e.g. 1second
	//bool reconnect;///A flag to indicate that we lost connection and we want to reconnect
	int num_client;
	static int cont_client;
	Thread<Client> connectionThread;

private: 
	//Make this class non-copyable, non-movable (the threads will become invalid)
	Client(const Client& client);///noncopyable constructor
	void operator=(const Client& client);///noncopyable =
};

};