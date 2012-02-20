// Server.cpp: implementation of the Server class.
//
//////////////////////////////////////////////////////////////////////
#include "servernode.h"
#include "server.h"
#include <stdlib.h>

using namespace std;
using namespace mr;

void ServerNode::handleMsg(const Message& message)
{
	if(message.isRequest())
	{		
		/*if(rand()%200==0)
		{	
			LOG_WARNING("empty answer");
			string response;
			Message responseMsg(response,TYPE_RESPONSE);
			sock.Send(responseMsg.getBuffer(),responseMsg.bufferSize());	//send response message
			return;
		}*/
		string response=handleRequest(message.getContent());		//call to virtual method
		Message responseMsg(response,TYPE_RESPONSE);
		sock.Send(responseMsg.getBuffer(),responseMsg.bufferSize());	//send response message
	}
	if(message.isData())
	{
		handleData(message.getContent());
	}
	if(message.isResponse())
	{
		LOG_ERROR("Server cannot receive responses");
		return;
	}
}
string ServerNode::handleRequest(const string& msg)
{
	return server->handleRequest(msg);
}
void ServerNode::handleData(const string& msg)
{
	server->handleData(msg);
}
void ServerNode::onDisconnect()
{
	server->disconnect(this);
}
