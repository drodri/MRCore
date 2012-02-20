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

#include "powercubeaxis.h"
#include <iostream>

#define STX 0x02
#define ETX 0x03
#define DLE 0x10
#define SENDDAT 0x04
#define RECVDAT 0x08

using namespace std;

namespace mr
{
//this union is only internal trick for this protocol, and thus it is ommitted from doxygen
/// \cond
union PQconvert{
PQBYTE Cbyte[4];
float Cfloat;
int Cint;
unsigned int CUint;
};
/// \endcond

/*****************************************************************
SendTelegram(PQBYTE *data, int sz)
sends telegram through the port. computes the BCC and includes 
de DLE correction
Parameters:
 data: external PQBYTE buffer.
 sz: data size.
return: NOERROR if OK
		ERRORSEND if the port is not able to transmit de datagram
		ERROR_DATA_SZ if the input data exceed the maximun
*****************************************************************/
int PowerCubeAxis::SendTelegram(PQBYTE *data, PQBYTE sz)
{
	PQBYTE telegram[30],datagram[30];
	unsigned int i,BCC=0;

	if(sz>8)return ERROR_DATA_SZ;
	telegram[0]=SENDDAT|tel1;
	telegram[1]=tel2|(sz&0x0F);
	for(i=0;i<sz;i++)telegram[2+i]=data[i];
	//calculo el BCC
	for(i=0;i<sz+2u;i++)
		BCC+=telegram[i];
	BCC=BCC+(BCC>>8);
	telegram[2+sz]=BCC&0xFF;

	//corrección para el DLE y los bytes intermedios
	PQBYTE *dest=datagram;

	dest[0]=STX;dest++;
	for(i=0;i<sz+3u;i++) //incluye el BCC
		{
		dest[0]=telegram[i];
		if(dest[0]==STX){dest[0]=DLE;dest[1]=0x82;dest++;}
		else if(dest[0]==ETX){dest[0]=DLE;dest[1]=0x83;dest++;}
		else if(dest[0]==DLE){dest[1]=0x90;dest++;}
		dest++;
		}
	dest[0]=ETX;dest++;
	i=(unsigned int)(dest-datagram); //total number of serial frame bytes

	//enviar por el puerto (si es que está) los i bytes de data
	//printf("Sending: %d\n",i);
	//for(int crec=0;crec<i;crec++)printf("%x ",Telegram[crec]);printf("\n");	
	if(port)if(port->send(i, datagram)!=PORT_OK)return ERRORSEND;	
	return NOERROR;
}
/*****************************************************************
GetTelegram(PQBYTE *data)
read the port until TIMEOUT or ETX detected
Parameters:
 data: external PQBYTE buffer.
 
return: data size if OK
		ERRORRECIEVE if the port is not able to transmit de datagram
		ERROR_DATA_SZ if the received data exceeds the maximun
		ERROR_CHK if checksum error
*****************************************************************/
int PowerCubeAxis::GetTelegram(PQBYTE *tel)
{
	PQBYTE datagram[30],*pdata;

	pdata=datagram;
	bool flag=true;
	if(!port)return ERRORRECIEVE; 
	while(flag){
		//de momento o se lee o no se lee, no identif el error
		if(port->receive(1,pdata)!=1)
		{
			cout<<"Timeout"<<endl;
			return ERRORRECIEVE;
		}
		if(pdata==datagram && pdata[0]!=STX)
			continue;
		if(pdata[0]==ETX)flag=false; //leida trama completa
		if(pdata-datagram>29)
			return ERROR_DATA_SZ;
			pdata++;
		}
	int tam,sz;
	tam=(unsigned int)(pdata-datagram); //trama transmitida
	pdata=datagram;
	for(int i=0;i<tam;i++){
		if(datagram[i]==DLE){
			if(++i<tam)datagram[i]=datagram[i]-0x80;
			else return ERRORRECIEVE;
			}
		*pdata=datagram[i];
		pdata++;
		}
	tam=(unsigned int)(pdata-datagram); //tamanio trama sin la correccion DLE
	if(datagram[0]!=STX)return ERRORRECIEVE;
	if(datagram[1]!=(RECVDAT|tel1))return ERRORRECIEVE;
	if((datagram[2]&0xF0)!=tel2)return ERRORRECIEVE;
	sz=datagram[2]&0x0F; //numero de bytes significativos que se esperan
	//verifico que el numero de bytes es 5+sz
	if(tam!=5+sz)return ERRORRECIEVE;
	//verifico el checksum;
	unsigned int BCC=0;
	for(int i=1;i<sz+2;i++)BCC+=datagram[i];
	BCC=BCC+(BCC>>8);
	if(datagram[3+sz]!=BCC)ERROR_CHK;
	//trama correcta, la copio y devuelvo los bytes significativos
	for(int i=0;i<sz;i++)tel[i]=datagram[3+i];
	return sz; 
}
/*****************************************************************
Reset

return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::Reset()
{
	PQBYTE data[8];
	data[0]=0x00;
	SendTelegram(data,1);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=1)return ERROR_ACK;
	if(data[0]!=0x00)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
Home

return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::Home()
{
	PQBYTE data[8];
	data[0]=0x01;
	SendTelegram(data,1);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=1)return ERROR_ACK;
	if(data[0]!=0x01)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
Stop

return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::Stop()
{
	PQBYTE data[8];
	data[0]=0x02;
	SendTelegram(data,1);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=1)return ERROR_ACK;
	if(data[0]!=0x02)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
SetSpeed
parmaters: speed in rad per second
return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::SetSpeed(float speed) //rad/s
{
	PQconvert val;
	val.Cfloat=speed;
	PQBYTE data[8];
	data[0]=0x08; //Command setExtended
	data[1]=0x4F; //target velocity
	data[2]=val.Cbyte[0];data[3]=val.Cbyte[1];data[4]=val.Cbyte[2];data[5]=val.Cbyte[3];
	SendTelegram(data,6);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=3)return ERROR_ACK;
	if(data[0]!=0x08)return ERROR_ACK;
	if(data[1]!=0x4F)return ERROR_ACK;
	if(data[2]!=0x64)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
SetAccel
parmaters: speed in rad per sqare second
return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::SetAccel(float accel) //Rad/s2
{
	PQconvert val;
	val.Cfloat=accel;
	PQBYTE data[8];
	data[0]=0x08; //Command set extended
	data[1]=0x50; //target accel
	data[2]=val.Cbyte[0];data[3]=val.Cbyte[1];data[4]=val.Cbyte[2];data[5]=val.Cbyte[3];
	SendTelegram(data,6);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=3)return ERROR_ACK;
	if(data[0]!=0x08)return ERROR_ACK;
	if(data[1]!=0x50)return ERROR_ACK;
	if(data[2]!=0x64)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
Move
parmaters: in rad 

return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::Move(float pos)	//Rad
{
	PQconvert val;
	val.Cfloat=pos;
	PQBYTE data[8];
	data[0]=0x0B;
	data[1]=0X04; //MOT_ID, Target Position
	data[2]=val.Cbyte[0];data[3]=val.Cbyte[1];data[4]=val.Cbyte[2];data[5]=val.Cbyte[3];
	SendTelegram(data,6);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=3)return ERROR_ACK;
	if(data[0]!=0x0B)return ERROR_ACK;
	if(data[1]!=0x04)return ERROR_ACK;
	if(data[2]!=0x64)return ERROR_ACK;
	return NOERROR;
}
/*****************************************************************
GetPos
  retrieves the angular position in radians.

return: NOERROR if ok
*****************************************************************/
int PowerCubeAxis::GetPos(float &value)
{
	PQconvert val;
	PQBYTE data[8];
	data[0]=0x0A;//Comando GetExtended
	data[1]=0X3C;//PAR_ID Caracter de Parameter ID (PAR_ID Actual Position = 0X3C)
	SendTelegram(data,2);
	int res=GetTelegram(data);
	if(res<0)return res;
	if(res!=6)return ERROR_ACK;
	if(data[0]!=0x0A)return ERROR_ACK;
	if(data[1]!=0x3C)return ERROR_ACK;
	val.Cbyte[0]=data[2];val.Cbyte[1]=data[3];
	val.Cbyte[2]=data[4];val.Cbyte[3]=data[5];
	value=val.Cfloat;
	return NOERROR;
}
}; //end namespace mr