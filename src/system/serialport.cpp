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

#include "serialport.h"
#include <fcntl.h>     // for low-level file open()

#ifdef WIN32
	#include <math.h>
#else
	#include <termios.h>    // for seral-port functions
	#include <math.h>      // for maths and M_PI
	#include <fstream>   // for file io
	#include <stdio.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
	#include <string.h>
#endif

#include <iostream>  // for standard IO

using namespace std;

namespace mr
{
SerialPort::SerialPort()
{
	#ifdef WIN32
		idCom=NULL;
	#else
		serialPort=-1;		
	#endif
}
int SerialPort::init (const char device[]) 
{
#ifdef WIN32
	idCom=CreateFile(device,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(idCom==INVALID_HANDLE_VALUE ) 
	{	
		idCom=NULL;
		printf("Error Opening port Win32\n");
		return(PORT_ERROR_CREATEFILE);
	}
	else
	{	
		GetCommState(idCom,&dcb);

		dcb.BaudRate=CBR_9600;
		dcb.fBinary=1;
		dcb.fOutxCtsFlow=0;
		dcb.fOutxDsrFlow=0;
		dcb.fDtrControl=1;////??????????
		//dcb.fDtrControl=DTR_CONTROL_DISABLE;////??????????
		dcb.fDsrSensitivity=0;
		dcb.fTXContinueOnXoff=1;
		dcb.fOutX=0;
		dcb.fInX=0;
		dcb.fErrorChar=0;
		dcb.fNull=0;
		dcb.fRtsControl=1;
		dcb.fAbortOnError=1;
		dcb.XonLim=2048;
		dcb.XoffLim=512;
		dcb.ByteSize=8;
		dcb.Parity=0;
		dcb.StopBits=0;
	//		dcb.StopBits=ONESTOPBIT;
		dcb.XonChar=17;
		dcb.XoffChar=19;
		dcb.ErrorChar=0;
		dcb.EofChar=0;
		dcb.EvtChar=0;
		dcb.DCBlength=sizeof(dcb);
		if(SetCommState(idCom,&dcb)==0)
		{
			CloseHandle(idCom);
			idCom=NULL;
			printf("Unable to configure port\n");
			return(PORT_ERROR_SETDCB);
		}
		timeouts.ReadIntervalTimeout=PORT_COMMS_INTERVAL_TIMEOUT;
		timeouts.ReadTotalTimeoutMultiplier=PORT_COMMS_TOTAL_TIMEOUT_MULTIPLIER;
		timeouts.ReadTotalTimeoutConstant=PORT_COMMS_TOTAL_TIMEOUT_CONSTANT;
		timeouts.WriteTotalTimeoutMultiplier=0;
		timeouts.WriteTotalTimeoutConstant=0;
		SetCommTimeouts(idCom,&timeouts);

	/*	unsigned long flus=0;
		int ret;
		do
		{
			char dummy[1000];
			ret=ReadFile(idCom,dummy,1000,&flus,NULL);
			if(flus!=0)
				printf("%d bytes have been flushed\n",flus);
		}while(flus!=0);*/

		return(PORT_OK);
	}
	
#else	
	if(serialPort!=-1)
		close();
  	struct termios serialSettings;
	serialPort=-1;
	serialPort = open (device, O_RDWR |O_NOCTTY);

	if(serialPort==-1) 
	{
		printf("Error al abrir el puerto\n");
		return PORT_ERROR;
	}

   	serialSettings.c_cc[VMIN] = 0;
  	serialSettings.c_cc[VTIME] = 1;
 
  	serialSettings.c_oflag = 0;
   	serialSettings.c_iflag = ( IGNPAR ) ;//DRL( IGNBRK | IGNPAR ) ;
   	serialSettings.c_cflag = ( CLOCAL| CREAD | B9600 | CS8 );
   	serialSettings.c_lflag = 0;   

   	cfsetospeed (&serialSettings, B9600);
   	cfsetispeed (&serialSettings, B9600);

   	if(tcsetattr (serialPort, TCSANOW, &serialSettings)!=0)
		return PORT_ERROR;
	else return PORT_OK;

#endif
}
void SerialPort::close () 
{
#ifdef WIN32
	CloseHandle(idCom);	
	idCom=NULL;
#else
	::close(serialPort);
	serialPort=-1;
#endif	
}
   

int SerialPort::receive ( int num,unsigned char * cad) 
{
	#ifdef WIN32
		unsigned long read=0;
		unsigned long read2=0;
		///LEo un primer cararcter
		if(ReadFile(idCom,cad,1,&read2,NULL)==0) 
			return PORT_ERROR_READFILE;
		if(read2!=1)return PORT_ERROR_TIMEOUT;
		//SI lo he leido bien sigo leyendo los que me piden
		if(num>1)//intento leer el resto si lo hay
		{
			if(ReadFile(idCom,cad+1,num-1,&read,NULL)==0)
				return PORT_ERROR_READFILE;
		}
		return(read+1);
	#else
		char car;
		int i=0;
	//	  printf("SE solicitan %d\n",num);
		while(read(serialPort,&car,1)==1)
		 {
		//printf("Leido 1\n");
		  cad[i]=car;
		  i++;
		   if(i>=num)break;
		}
		// int leidos= read(serialPort,cad,num);
		int leidos=i;

	//	  printf("Se han leido %d\n",leidos);
		 return leidos;
	#endif	
}
   
int SerialPort::send (int n,unsigned char * cad) 
{
	#ifdef WIN32
		unsigned long written=0;
		if(WriteFile(idCom,cad,n,&written,NULL)==0) return(PORT_ERROR_WRITEFILE);
		if(written!=n) 
			return(PORT_ERROR_WRITENUM);
		return(PORT_OK);
	#else
		// printf("Enviando %d\n",n);
		int num=write(serialPort, cad, n);
		if(num!=n)
			return PORT_ERROR_WRITENUM;
		return PORT_OK;
	#endif	
}


int SerialPort::setBaud (int baud) 
{	
	#ifdef WIN32
		dcb.BaudRate=baud;

		if(SetCommState(idCom,&dcb)==0)
			return(PORT_ERROR_SETDCB);
		return(PORT_OK);
	#else
		struct termios serialSettings;
	
   		serialSettings.c_cc[VMIN] = 0;
  		serialSettings.c_cc[VTIME] = 1;
 
  		serialSettings.c_oflag = 0;
   		serialSettings.c_iflag = ( IGNBRK | IGNPAR ) ;
   		serialSettings.c_cflag = ( CLOCAL| CREAD | B9600 | CS8 );
   		serialSettings.c_lflag = 0;   
		
   		if(baud==38400)
		{
			cfsetospeed (&serialSettings, B38400);
   			cfsetispeed (&serialSettings, B38400);
		}    
		else if(baud==9600)
		{
			  cfsetospeed (&serialSettings, B9600);
   			cfsetispeed (&serialSettings, B9600);
		}
		else if(baud==115200)
		{
			  cfsetospeed (&serialSettings, B115200);
   			cfsetispeed (&serialSettings, B115200);
		}
		else if (baud==500000)
		{
			cfsetospeed (&serialSettings, B500000);
   			cfsetispeed (&serialSettings, B500000);
		}
   		if(tcsetattr (serialPort, TCSANOW, &serialSettings)!=0)
			return PORT_ERROR;
		else 
			return PORT_OK;
	#endif	
}

   
}; //end namespace mr
