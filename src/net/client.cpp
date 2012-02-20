// Client.cpp: implementation of the Client class.
//
//////////////////////////////////////////////////////////////////////

#include "client.h"
#include "system/thread.h"
#include "base/logger.h"
#include "message.h"
#include "system/time.h"
#include <sstream>
#include <iostream>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace mr;

Client::Client():semafore(0,1)
{
	closeClient=false;
//	statusRequest=0;
//	 // Create a semaphore with initial and max counts of 1

	host_port=-1;
}

Client::~Client()
{
	close();
}

int Client::connect(const char address[],int port)
{
	closeClient=false;
	std::stringstream stre;
	host_addr=address;
	host_port=port;
	stre<<address<<":"<<port;
	host=stre.str();
	closeNode=false;
	if(!sock.connect(address,port))
		closeNode=true;
	else
	{
		startThread();
	}

	Thread<Client> t;
	t.Start(&Client::Reconnect, this,0);
	return 1;
}
/*void Client::onDisconnect()
{
	if(!closeClient)
	{
		Sleep(100);
		if(!sock.IsValid() )
		{
			LOG_INFO("Trying to reconnect...");
			closeNode=false;
			
			if(!sock.connect(host_addr.c_str(),host_port))
				closeNode=true;
			Sleep(250);
			startThread();//launch thread in any case, so it will call onDisconnect again
		}
		else
			LOG_ERROR("On Disconnect should not be here");
	}
}*/
void Client::onDisconnect()
{
	LOG_INFO("Client disconnected");
/*	if(!closeClient)
	{	
		Thread<Client> t;
		t.Start(&Client::Reconnect, this,0);
	}*/
}
void* Client::Reconnect(void* d)
{
	while(!closeClient)
	{
		Sleep(10);
		if(!sock.IsValid())
		{	
			receiveThread.Join();//make sure the thread has finished
			closeNode=false;
			Sleep(250);//leave some time to the server
			//This connect in Win can spend up to 1 second
			if(!sock.connect(host_addr.c_str(),host_port))
			{
				closeNode=true;		
			}
			else
			{
				LOG_INFO("Client reconnected");
				//Sleep(250);
				startThread();//launch thread in any case, so it will call onDisconnect again
			}	
		}
	}
	LOG_INFO("Finished reconnect thread");
	return 0;
}
void Client::close()
{
	closeClient=true;
	host="";
	NetNode::close();
}

int Client::sendRequest(string request,string& response,int timeout)
{
	if(!sock.IsValid())
		return COMM_ERROR;

	responseBuffer="";//Important: clear responseBuffer
	Message msg(request,TYPE_REQUEST);
	int len=sock.Send(msg.getBuffer(),msg.bufferSize());	
	if(len!=msg.bufferSize())
		return COMM_ERROR;

	//Lets wait until receivesMessages signals that a response has arrived
	 // Wait for main program to release the semaphore
	if(!semafore.wait(timeout))
	{
		//if client is not connected but server is up, it fails here
		//due that the socket is not invalid, it is in the listen queue	
		LOG_WARNING("Wait failed");
		response="";
		NetNode::close();
	//	semafore.signal();
		return COMM_ERROR;
	}
	else 
		response=responseBuffer;
	
	return COMM_OK;
}
void Client::handleMsg(const Message& message)
{
	if(message.isData())
	{
		handleData(message.getContent());
	}
	else if(message.isResponse())
	{
		responseBuffer=message.getContent();
		semafore.signal();
	}
	else
	{
		LOG_ERROR("Client received bad message");
	}
}
