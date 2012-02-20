/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
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

#ifndef __POWERCUBEAXES_H_
#define __POWERCUBEAXES_H_

//Exernal Includes

//Internal Includes
#include "../../system/serialport.h"

//defines
#define NOERROR	0
#define ERRORNULL		-201
#define ERRORSEND		-202
#define ERRORRECIEVE	-203
#define ERROR_DATA_SZ   -204
#define ERROR_CHK		-205
#define ERROR_ACK		-206
#define ERROR_PORT		-207


namespace mr
{
typedef unsigned char PQBYTE;

class PowerCubeAxis
{
	SerialPort *port;   //Serial port. Open & Close is out of the class responsability.
	PQBYTE moduleID; //module Address
	PQBYTE tel1,tel2; //bytes encoding moduleID
	int SendTelegram(PQBYTE *data, PQBYTE sz); //sends telegram: computes the BCC and includes de DLE correction 
	int GetTelegram(PQBYTE *tel); //gets the ACK, and extracts the real DATA

public:
	void SetComPort(SerialPort *p){port=p;}
	PowerCubeAxis():port(0),moduleID(0),tel1(0),tel2(0){};
	void SetID(PQBYTE id){moduleID=id;tel1=(moduleID&0x18)>>3;tel2=(moduleID&0x07)<<5;}
	int Reset();
	int Home();
	int Stop();
	int SetSpeed(float Speed); //rad/s
	int SetAccel(float Accel); //Rad/s2
	int Move(float Mov);	//Rad
	int GetPos(float &value);//obtain angle in rad

};

}; //end namespace mr

#endif
