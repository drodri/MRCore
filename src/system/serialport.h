/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/

#ifndef __SERIALPORT_HH
#define __SERIALPORT_HH

#ifdef WIN32
	#define _WINSOCKAPI_ //prevent the inclusion of sockets
	#include<windows.h>
#else
#endif


//CONFIGURATION PARAMETERS
#define PORT_COMMS_INTERVAL_TIMEOUT			150	//millisecs
#define PORT_COMMS_TOTAL_TIMEOUT_MULTIPLIER 150	//millisecs
#define PORT_COMMS_TOTAL_TIMEOUT_CONSTANT	0   //millisecs
	

//ERROR CODES
#define PORT_OK 0
#define PORT_ERROR -1
#define PORT_ERROR_CREATEFILE		-101
#define PORT_ERROR_SETDCB			-102
#define PORT_ERROR_WRITEFILE		-103 
#define PORT_ERROR_WRITENUM			-104 
#define PORT_ERROR_READFILE			-105
#define PORT_ERROR_READNUM			-106
#define PORT_ERROR_CLOSE_HANDLE		-107
#define PORT_ERROR_BAD_PARAMETER	-108
#define PORT_ERROR_TIMEOUT			-109

namespace mr
{
/**class SerialPort is a multi OS serial port wrapper, useful for communicating
with serial devices as laser, robots... **/
class SerialPort
{	
public:
	SerialPort();
	/** The init() function opens the port
	\param device is the string of the port: "/dev/ttyS1" in linux, "COM1" in win
	\return -1 on error **/
	int init(const char device[]);  
	/** Closes an opened port **/
	void close();
	int setBaud(int baud);
	int receive(int num,unsigned char* cade);
	int send(int num,unsigned char* cade);
	
protected:

#ifdef WIN32
	DCB dcb;
	HANDLE  idCom;
	COMMTIMEOUTS timeouts;
#else
	  int serialPort;
#endif
};
    
}; //end namespace mr

#endif /*PORT*/