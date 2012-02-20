/////////////////////////////////////////////////////////////////////////////
// Socket.h header file
//
// Copyright (C) 2010. Diego Rodriguez-Losada. All rights reserved
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.
/////////////////////////////////////////////////////////////////////////////
#ifndef __MRCORE__SOCKET_H_
#define __MRCORE__SOCKET_H_
#pragma once



//The default socket buffer in WinXP is 17520 = 1.7 x 1000 which can be too small for large transfer
//The following can adjust the buffer size, use values of 128, 256 ... from 256 to 1024 (1Mb) there is no difference in performance
#define TCP_BUFFERS_SIZE 262144   //256x1024

#ifdef _WIN32
	#include <winsock2.h>	
	#pragma comment (lib, "ws2_32.lib") 
#else
	#include <netdb.h>   
	#include <sys/time.h> 
	#include <sys/types.h>    
	#include <sys/socket.h>
	#include <sys/select.h>	
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
//	#include <pthread.h>
	#include <errno.h>	
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define SD_BOTH SHUT_RDWR
	#define closesocket(x) ::close(x)
	typedef int SOCKET;
#endif

#include "mutex.h"
#include <iostream>
#include <string>

#define SOCKET_SERVER 0
#define SOCKET_CLIENT 1

///The socket class is non-copyable, no 2 sockets can be the same
///But it is movable
using namespace std;
namespace mr
{
//#define Sleep(x) usleep(x*1000)
#ifndef _WIN32
	void Sleep(int);
#endif

class Socket 
{
public:
	Socket();
	virtual ~Socket();
	
	///Move semantics constructor and assignment. The source socket will become INVALID_SOCKET
	Socket(Socket& s);
	void operator=(Socket& s);

	///server side function wrappers
	Socket* acceptConnection();//Linux(gcc) does not accept return by value for move semantics	
	int initServer(int port, const char address[]="");

	///client side connect wrappper
	bool connect(const char address[],int port);

	virtual int close();
	int IsConnected();
	bool IsValid(){return sock!=INVALID_SOCKET;}

	//raw send-receive, returns the number of bytes send, -1 if error
	int Send(const char* cad,int length);

	//this one defines the maximum length of the buffer
	//but do not try to wait until "length" bytes effectively arrive
	int Receive(char* cad,int length,int timeout=50);//non-blocking reception (with select), blockign with timeout<0
	//this one tries to receive length bytes, will retry until timeout
	bool ReceiveBytes(char* cad,int length, int timeout);

	std::string GetIP(){return ip;}
	int GetPort(){return port;}
	
protected:
	///Increment the size of the socket buffer
	static void setBuffersSize(SOCKET s,int size);
	///handles error, if the socket is connected close it
	void error(const char* cad="");

	SOCKET sock;//the socket used for the send-receive
	
	//the address of the socket
	std::string ip;
	int port;
private:
	Mutex sendMutex;
//	Mutex receiveMutex;
	static int count;//counter for WSStart WSClose

};

};

#endif
