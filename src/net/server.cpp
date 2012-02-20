// Server.cpp: implementation of the Server class.
//
//////////////////////////////////////////////////////////////////////
#include "server.h"
#include "base/logger.h"
#include "system/thread.h"
#include <iostream>
#include <sstream>
#include "message.h"
#include "time.h"
#include "servernode.h"
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace mr;

Server::Server(string name)//:semNodes(0,50)
{
	max_num_clients=0;
	closeServer=false;
	serverName=name;
}

Server::~Server()
{
	close();
}

int Server::init(int port,  int max_clients,bool threaded,const char address[])
{
	socket_server.initServer(port,address);
	max_num_clients=max_clients;
	closeServer=false;
//	for(unsigned int i=0;i<max_num_clients;i++)
//		semNodes.signal();
		
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

	//close nodes (wait to thread finish included)
	for(unsigned int i=0;i<nodes.size();i++)
		nodes[i]->close();//will call disconnect and empty the vector of nodes

	if(nodes.size()!=0)
		LOG_ERROR("Node vector should be empty!");

	//wait here until all threads correctly close
	LOG_INFO("Server "<<serverName<<" close succesfully");
}
void Server::disconnect(ServerNode* nod)
{
	mutexNodes.Lock();
	for(unsigned int i=0;i<nodes.size();i++)
	{
		if(nodes[i]==nod)
		{
			delete nod;
			nodes.erase(nodes.begin()+i);
//			semNodes.signal();
			LOG_INFO("Server "<<serverName<<" Disconnection. Current number "<<nodes.size());
			mutexNodes.Unlock();
			return;
		}
	}
	mutexNodes.Unlock();
	LOG_ERROR("Error this node not existing!!!!!!!!");
}
void* Server::handleConnections(void* )
{
	LOG_INFO("Server "<<serverName<<" running");
	while(!closeServer)
	{
//		if(!semNodes.wait(-1))//forever
//		{
//			LOG_ERROR("SemNodes unblock failure");
//			break;
//		}
		Socket* temp=socket_server.acceptConnection();	
		if(!temp || !temp->IsConnected())
		{
			if(!closeServer)
				LOG_ERROR("Server "<<serverName<<" Accept returned invalid socket");
			break;
		}
		mutexNodes.Lock();
		if(nodes.size()>=max_num_clients)
		{
			//LOG_ERROR("Should not have more nodes!!!!!");
			temp->close();
			delete temp;
			LOG_WARNING("Server "<<serverName<<" Discarded connection, we are up to the max");
			mutexNodes.Unlock();
			continue;
		}
		mutexNodes.Unlock();

		ServerNode* nod=new ServerNode(this, *temp);
		delete temp;
		mutexNodes.Lock();
		nodes.push_back(nod);
		nod->startThread();
		mutexNodes.Unlock();
		LOG_INFO("Server "<<serverName<<" Connection. Current number : "<<nodes.size());
	}

	return 0;
}
void Server::sendData(const string& msg)
{
	for(unsigned int i=0;i<nodes.size();i++)
		nodes[i]->sendData(msg);
}



