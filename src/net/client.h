// Client.h: interface for the HttpClient class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __MRCORE_CLIENT_H_INCLUDED_
#define __MRCORE_CLIENT_H_INCLUDED_
#pragma once

#include "netnode.h"
#include "system/mutex.h"
#include "system/semaphore.h"

#include <string>

namespace mr
{

class Client : public NetNode
{
public:
	Client();
	virtual ~Client();

	//wrappers of socket connection
	int connect(const char address[]="127.0.0.1",int port=80);	
	void close();

	//returns IP:port address of the server
	std::string getHost(){return host;}
	std::string getAddress(){return host_addr;}
	int getPort(){return host_port;}

protected:
	virtual void handleMsg(const Message& msg);
	///main function for client-server comm. The client string is converted to a message, sent
	///the server response message is received, the message content is returned in "response"
	///timeout in milliseconds
	///return OK, error_com, error_mensaje
	int sendRequest(string request,string& response,int timeout=50);
//	Mutex requestMutex;
//	int statusRequest;
	string responseBuffer;
	
	virtual void onDisconnect();//inform this thread has terminated
	void* Reconnect(void*);
	Semaphore semafore;
	//Mutex mutex;//so the http_client is thread_safe.

	std::string host;
	std::string host_addr;
	int host_port;
	
private: 
	bool closeClient;
	//Make this class non-copyable, non-movable (the threads will become invalid)
	Client(const Client& client);///noncopyable constructor
	void operator=(const Client& client);///noncopyable =
};

};

#endif