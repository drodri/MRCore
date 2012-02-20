// Socket.cpp: implementation of the Socket class.
//
//////////////////////////////////////////////////////////////////////

#include "socket.h"
#include "time.h"
#include "base/serialize.h"
#include "base/logger.h"
//#include "Utils/MRTime.h"

#include <iostream>
#include <sys/timeb.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace mr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int Socket::count=0;

namespace mr{
#ifndef _WIN32
void Sleep(int x){
	usleep(x*1000);
}
#endif
}

Socket::Socket()
{
	ip="";
	port=0;
	if(count==0)
	{
		//Open socket system
		#ifdef _WIN32
			WSADATA wsaData;
    
			/* first try Winsock 2.0 */
			if(WSAStartup(MAKEWORD(2, 2),&wsaData) == 0)
			{
				int v1=LOBYTE( wsaData.wVersion ) ;
				int v2=HIBYTE( wsaData.wVersion );
				if ( v1 != 2 || v2 != 2 ) 
				{
					// Tell the user that we could not find a usable WinSock DLL.  
					Logger(LOGGER_ERROR).Log()<<"Windows Sockets Not Existing";
					WSACleanup( );
				}
			}
		#endif
	}
	count++;
	sock=INVALID_SOCKET;
}
Socket::Socket(Socket &s)
{
	ip=s.ip;
	port=s.port;
	sock=s.sock;
	count++;

	s.ip="";
	s.port=0;
	s.sock=INVALID_SOCKET;
}
void Socket::operator=(Socket& s)
{
	ip=s.ip;
	port=s.port;
	sock=s.sock;
	count++;

	s.ip="";
	s.port=0;
	s.sock=INVALID_SOCKET;
}
Socket::~Socket()
{
	count--;
	if(count==0) //close socket system
	{
		#ifdef _WIN32
			WSACleanup();
		#endif
	}
}
int Socket::initServer(int porti,const char address[])
{
	port=porti;
	ip=address;
	if(sock!=INVALID_SOCKET)
	{
		Logger(LOGGER_WARNING).Log()<<"Warning: Socket already opened";
		close();
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock==INVALID_SOCKET)
	{
		error("Server: Error Socket");
		return -1;
	}

	char on=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	if(strlen(address)==0)
		server_address.sin_addr.s_addr = INADDR_ANY;
	else
		server_address.sin_addr.s_addr = inet_addr(address);
	server_address.sin_port = htons(port);


	int len = sizeof(server_address);
	if (bind(sock,(struct sockaddr *) &server_address,len) < 0)
	{
		Logger(LOGGER_ERROR).Log()<<"Server: bind";
		return -1;
	}
	setBuffersSize(sock,TCP_BUFFERS_SIZE);
	// Damos como máximo 5 puertos de conexión.
	if (listen(sock,0) < 0)
	{
		Logger(LOGGER_ERROR).Log()<<"Server: listen";
		return -1;
	}	
		
// Get the local hostname IP
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry=gethostbyname(szHostName);
	if(host_entry!=NULL)
	{
		char * szLocalIP = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
		ip=std::string(		szLocalIP	);
	}
	else
		LOG_ERROR("Unable to get IP of "<<szHostName);
	return 0;
}
Socket* Socket::acceptConnection()
{
	Socket* ret=NULL;
	if(sock==INVALID_SOCKET)	
		return ret;

	struct sockaddr_in client_address;
//	LOG_INFO("Accept: Accepting connections...");
	#ifdef _WIN32
		int len = sizeof(client_address);
	#else
		unsigned int len = sizeof(client_address);
	#endif


	SOCKET aux_sock= accept(sock,(struct sockaddr *)&client_address, &len);
	if(aux_sock==INVALID_SOCKET)
	{
		error();
		return ret;
	}
	ret=new Socket();
	ret->sock=aux_sock;
	LOG_INFO("Accept: ClientAddress "<< inet_ntoa(client_address.sin_addr)<<" Port "<< ntohs(client_address.sin_port));

	return ret;
}
bool Socket::connect(const char address[], int port)
{
	close();//Check it
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(address);
	server_address.sin_port = htons(port);

	SOCKET aux_sock=socket(AF_INET, SOCK_STREAM,0);
	if (aux_sock == INVALID_SOCKET) 
	{
		error("Client: Error create socket");
		return false;
	}

	setBuffersSize(aux_sock,TCP_BUFFERS_SIZE);

//	char on=1;
//	setsockopt(aux_sock,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof(on));
//	unsigned long block=1;
//	ioctlsocket(aux_sock,FIONBIO,&block);

	int len= sizeof(server_address);
	int con_ret=::connect(aux_sock,(const struct sockaddr *) &server_address,len);
	if(con_ret!=0) 
	{	
//		fd_set readfds;FD_ZERO(&readfds);FD_SET(aux_sock,&readfds);
//		struct timeval tout;tout.tv_sec=0;tout.tv_usec=50000;
//		int reads=select (aux_sock+1,NULL,&readfds,NULL,&tout );
//		if(reads!=1)
//		{
		//	error("Client could not connect");
			error();
			return false;
//		}
	}

	//set back to blockking
//	block=0;
//	ioctlsocket(aux_sock,FIONBIO,&block);

	//finish
	sock=aux_sock;
	return true;
}
int Socket::close()
{
	shutdown(sock,SD_BOTH);
	#if _WIN32
		closesocket(sock); //select wont finish and block forever if we close the socket this way in linux
	#endif
	sock=INVALID_SOCKET;
	
	return 1;
}

int Socket::Send(const char *cad, int length)
{
	if(sock==INVALID_SOCKET)
		return -1;
		
	sendMutex.Lock();
	#ifdef _WIN32
		int err=send(sock, cad, length,0);
	//	int err1=send(sock, cad, length/2,0);
	//	Sleep(10);
	//	int err=send(sock, &cad[length/2], length-length/2,0);
	//	int err=err1+err2;
	#else
		#ifdef MACOSX
			int err=send(sock, cad, length,SO_NOSIGPIPE);
		#else
			int err=send(sock, cad, length,MSG_NOSIGNAL);
		#endif
	#endif
	sendMutex.Unlock();
	if (err == SOCKET_ERROR )
	{
		error("Send Error");
		return -1;
	}
	if(err!=length)
	{
		error("Send incomplete");
	//	MessageBox(NULL,"Hola","Hola",MB_OK);
		return -2;
	}
	return err;
}

bool Socket::ReceiveBytes(char *cad, int length,int timeout)
{
	if(sock==INVALID_SOCKET)
		return false;

	int l=0;
	int count=0;

	MRTime timer;
	timer.tic();
	do
	{	
		int ret=Receive(cad+l,length-l,10);
		if(ret==-1)
			return false;
		l+=ret;			
		if(timer.toc()>=timeout)
			return false;
	}
	while(l!=length);

	return true;
}
int Socket::Receive(char *cad, int length,int timeout)
{
	if(sock==INVALID_SOCKET)
		return -1;


	fd_set readfds;FD_ZERO(&readfds);FD_SET(sock,&readfds);
	int ret;
	if(timeout<0)//blocking
	{
		ret=select ((int)sock+1,&readfds,NULL,NULL,0 );//linux block for ever even if the socket is closed
	}
	else
	{	//non blocking
		struct timeval tout;tout.tv_sec=0;tout.tv_usec=timeout*1000;
		ret=select ((int)sock+1,&readfds,NULL,NULL,&tout );
	}

	if(1==ret)
	{	
		int r=recv(sock,cad,length,0); //If not length-1, overflows and creates weird memory errors
		if(r==SOCKET_ERROR || r==0)
		{	
		//	LOG_WARNING("Reception interrupted. Socket disconnected");
			error();
			return -1;		
		}
		else
		{	
			
			return r;	
		}	
	}
	return 0;

	/*BLOCKING
		if(sock==INVALID_SOCKET)
		return -1;
	int r=recv(sock,cad,length,0);
	if(r<=0)
	{	
		std::cout<<"Receive bytes Error"<<std::endl;
		sock=INVALID_SOCKET;
		return -1;		
	}
	return r;*/
}

int Socket::IsConnected()
{
	if(sock==INVALID_SOCKET)
		return 0;
	else
	{
		fd_set readfds;FD_ZERO(&readfds);FD_SET(sock,&readfds);
		struct timeval tout;tout.tv_sec=0;tout.tv_usec=0;
		int reads=select ((int)sock+1,&readfds,NULL,NULL,&tout );
		if(reads==1)
		{
			char cad[100];
			int r=recv(sock,cad,0,0); 
			if(r==SOCKET_ERROR )
			{
				close();
				return 0;
			}
		//	return 1;
		}
	
		return 1;
	}
}
void Socket::error(const char* cad)
{
	if(strlen(cad)>0)
	{
		std::ostringstream log;
		log<<"Error: "<<cad<<std::endl;
		#ifdef _WIN32
		log<<"WSAError "<<WSAGetLastError();
		#else
		log<<"Error "<<strerror(errno);
		#endif
		LOG_ERROR(log.str());
	}
	if(sock!=INVALID_SOCKET)
	{	
		close();
	}
}

void Socket::setBuffersSize(SOCKET s,int window_size)
{
	// These setsockopt()s must happen before the connect() 
	int ret22=setsockopt(s, SOL_SOCKET, SO_SNDBUF,(char *) &window_size, sizeof(window_size));
	if(ret22!=0)
		LOG_ERROR("Set socket send buffer size");

	ret22=setsockopt(s, SOL_SOCKET, SO_RCVBUF,(char *) &window_size, sizeof(window_size));
	if(ret22!=0)
		LOG_ERROR("Set socket receive buffer size");
}
