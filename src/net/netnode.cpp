// Server.cpp: implementation of the Server class.
//
//////////////////////////////////////////////////////////////////////
#include "netnode.h"
#include "message.h"
#include "time.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace mr;

NetNode::NetNode()
{
	closeNode=false;
}

NetNode::~NetNode()
{
	
}
//asyncr message, streaming. The other node will receive in its handleMsg()
int NetNode::sendData(const string& str)
{
	if(!sock.IsValid())
		return COMM_ERROR;

	Message msg(str,TYPE_DATA);
	int len=sock.Send(msg.getBuffer(),msg.bufferSize());
	if(len!=msg.bufferSize())
		return COMM_ERROR;
	return COMM_OK;
}

void NetNode::close()
{
	if(closeNode)
		return;
	sock.close();
	closeNode=true;
	receiveThread.Join();
}
void NetNode::startThread()
{
	receiveThread.Start(&NetNode::receiveMessages,this,0);
}
void* NetNode::receiveMessages(void* d)
{
	while(!closeNode)
	{
		char header[HEADER_SIZE];	
		//Blocking reception (last param=-1)
		int ret=sock.Receive(header,HEADER_SIZE,-1);
		if(-1==ret)
		{
			//LOG_ERROR("This node receive error");//sock.Receive outputs LOG
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
		handleMsg(message);
	}
	sock.close();
	
	onDisconnect();//inform this thread has terminated, if server, will eliminate this node from the vector
	//SO NO REFERENCE to "this" should be done beyond this point
	return NULL;
}

