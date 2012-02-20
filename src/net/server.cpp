// Server.cpp: implementation of the Server class.
//
//////////////////////////////////////////////////////////////////////
#include "server.h"
#include "base/logger.h"

#include "system/time.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mr
{
Server::Server(string name)
{
	max_num_clients=0;
	closeServer=false;
	serverName=name;
}

Server::~Server()
{
}

int Server::init(int port,  int max_clients,bool threaded,const char address[])
{
	socket_server.initServer(port,address);
	max_num_clients=max_clients;
	closeServer=false;
	
	if(threaded)
		serverThread.Start(&Server::handleConnections,this);	
	else
		handleConnections();

	return 0;
}
void Server::close()
{
	if(closeServer) return;

	closeServer=true;
	socket_server.close();

	//wait here for handleConnections to finish
	serverThread.Join();

	for(unsigned int i=0;i<sockClients.size();i++)
	{
		sockClients[i]->close();
		delete sockClients[i];
	}

	for(unsigned int i=0;i<clients.size();i++)
		clients[i].Join();//close();//will call disconnect and empty the vector of nodes

	//wait here until all threads correctly close
	LOG_INFO("Server "<<serverName<<" close succesfully");
}
void* Server::handleConnections(void *d)
{
	while(!closeServer)
	{
		Socket* temp=socket_server.acceptConnection();
		if(!temp || !temp->IsConnected())
		{
			LOG_ERROR("Accept returned invalid socket");
			return NULL;
		}
		
		Thread<Server> t;
		t.Start(&Server::handleClient,this,temp);
		
		mutex.Lock();
		sockClients.push_back(temp);
		clients.push_back(t);	
		mutex.Unlock();
	}
	return NULL;
}

void* Server::handleClient(void *d)
{
	Socket* aux_sock=(Socket*)d;
	Socket sock=*aux_sock;

	while(!closeServer)
	{		
		char header[HEADER_SIZE];	
		//Blocking reception (last param=-1)
		int ret=sock.Receive(header,HEADER_SIZE,-1);
		if(-1==ret)
		{
			LOG_ERROR("Receive error");//sock.Receive outputs LOG
			break;
		}
		if(ret==0)
		{
			LOG_ERROR("SHould never be here");
			continue;
		}
		Message message;//buffering
		if(!message.addHeader(header,ret))
		{
			LOG_ERROR("Incorrect header "<<ret<<" "<<string(header,ret));
			break;
		}
		const int contentSize=message.getContentSize();
		char* buffer=new char[contentSize];
		int timeout=100+contentSize/10;
		if(!sock.ReceiveBytes(buffer,contentSize,timeout))
		{
			LOG_ERROR("Unable to get full message");
			break;
		}
		message.addContents(buffer,contentSize);
		delete buffer;
		
		string response=handleRequest(message.getContent());//call to virtual method
		Message responseMsg(response);
		sock.Send(responseMsg.getBuffer(),responseMsg.bufferSize());	//send response message
		
	}
	sock.close();

	mutex.Lock();
	for(int i=sockClients.size()-1;i>=0;i--)
	{
		if(sockClients[i]==aux_sock)
		{
			sockClients.erase(sockClients.begin()+i);
			clients.erase(clients.begin()+i);
			LOG_INFO("Closing thread and socket");
			break;
		}
	}
	mutex.Unlock();

	LOG_INFO("Server: Finishing client. NumClients: "<<sockClients.size());	
	return NULL;
}


};