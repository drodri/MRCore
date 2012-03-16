// Client.cpp: implementation of the Client class.
//
//////////////////////////////////////////////////////////////////////

#include "client.h"
#include "system/thread.h"

#include <sstream>
#include <iostream>

using namespace std;

namespace mr
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int Client::cont_client=0;

Client::Client()
{
	wantReconnect=true;
//	reconnect=false;

	cont_client++;
	num_client=cont_client;	
}

Client::~Client()
{
	
}

int Client::connect(const char address[],int port,bool wantRec)
{
	std::stringstream stre;
	host_addr=address;
	host_port=port;
	stre<<address<<":"<<port;
	host=stre.str();
	int ret=sock.connect(address,port);	

	wantReconnect=wantRec;
//	reconnect=false;
	if(wantReconnect)
		connectionThread.Start(&Client::ReconnectLoop,this,NULL);

	return ret;
}
void* Client::ReconnectLoop(void* d)
{
	while(wantReconnect)
	{
		Sleep(100);
	//	LOG_INFO("I want reconnect "<<sock.IsValid()<<" rec; "<<reconnect);
		if(!sock.IsValid()) //&& reconnect)
		{
			LOG_INFO("Trying to reconnect");
			Socket aux_sock;
			aux_sock.connect(host_addr.c_str(),host_port);
			mutex.Lock();
			sock=aux_sock;
			mutex.Unlock();
		}
	}

	return NULL;
}
void Client::close()
{
	host="";
	sock.close();
	wantReconnect=false;
	connectionThread.Join();
	LOG_INFO("Client correctly closed");
}
int Client::sendRequest(string request,string& response,int timeout)
{
	if(sock.IsValid())
	{
		mutex.Lock();
//		if(!sock.IsConnected() && reconnect && want_reconnect)
//			sock.Connect(host_addr.c_str(),host_port);

		Message message(request);
		sock.Send(message.getBuffer(),message.bufferSize());
		response=WaitResponse(timeout);
		mutex.Unlock();
		return COMM_OK;
	}
	
	return COMM_ERROR;
}

string Client::WaitResponse(int timeout)
{
	char header[HEADER_SIZE];	
	//Blocking reception (last param=-1)
	int ret=sock.Receive(header,HEADER_SIZE,timeout);
	if(-1==ret || ret==0)
	{
		LOG_ERROR("Receive error");//sock.Receive outputs LOG
//		reconnect=true;
		return "";
	}
	
	Message message;//buffering
	if(!message.addHeader(header,ret))
	{
		LOG_ERROR("Incorrect header "<<ret<<" "<<string(header,ret));
//		reconnect=true;
		return "";
	}
//	LOG_INFO("Waiting: "<<message.getContentSize());
	const int contentSize=message.getContentSize();
	char* buffer=new char[contentSize];
	int timeout2=100+contentSize/10;
	if(!sock.ReceiveBytes(buffer,contentSize,timeout2))
	{
		LOG_ERROR("Unable to get full message");
//		reconnect=true;
		delete buffer;
		return "";
	}
//	LOG_INFO("Received: "<<message.getContentSize());
	message.addContents(buffer,contentSize);
	delete buffer;

	return message.getContent();
}
};