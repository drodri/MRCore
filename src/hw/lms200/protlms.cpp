/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Diego Rodriguez-Losada
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

#include <stdio.h>
#include "protlms.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <iostream>

using namespace std;

#define CRC16_GEN_POL 0x8005

#ifdef WIN32

#else
	#include <sys/stat.h>
	#include <fcntl.h>


	#include <time.h>
	#include <signal.h>


	#include <string.h>
	#include <stdlib.h>
	#include <sys/types.h>


	#include <unistd.h>
	#include <pthread.h>
	#include <sys/time.h>
	#include <getopt.h>
#endif


namespace mr
{
/*************************************************************
Funcion CHECKSUM realizada por SICK
********************************************************/
//this union is only internal trick for this protocol, and thus it is ommitted from doxygen
/// \cond

struct caracter_entero
{
      unsigned char caracter_low;
      unsigned char caracter_high;
};

union dos_bytes
{
      unsigned short int entero;
      struct caracter_entero car_ent;
};
/// \endcond

static unsigned short int CreateCRC(unsigned char *CommData,unsigned int uLen)
{
unsigned short int uCrc16;
unsigned char abData[12];
uCrc16=0;
abData[0]=0;
while(uLen--)
{
    abData[1]=abData[0];
    abData[0]=*CommData++;
    if(uCrc16 & 0x8000)
    {
       uCrc16<<=1;
       uCrc16^=CRC16_GEN_POL;
    }
    else
    {
       uCrc16<<=1;
    }
    uCrc16^= *(unsigned short int *)&abData;
}

return(uCrc16);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ProtLMS::ProtLMS()
{
	telegram_out[0]=0x02;
	telegram_out[1]=0x00;
	for(int i=0;i<401;i++)
	{
		sen_alfa[i]=(float)sin((40.0f+i*0.25f)*M_PI/180.0f);
		cos_alfa[i]=(float)cos((40.0f+i*0.25f)*M_PI/180.0f);
	}
	mode=PROT_LMS_MODE_NOT_INIT;
	max_range=PROT_LMS_MAX_RANGE_8M;
	contin=false;
}

ProtLMS::~ProtLMS()
{
	port.close();
}
void ProtLMS::syncronizeFromPreviousCrash()
{
		unsigned char c[5];
	memset(c, 0, 5);
	int j;
	for (j = 0; j < 1000; j++)
	  {
	    for (int k = 0; k < 4; k++)
		c[k] = c[k + 1];
	    if (!port.receive(1, c+4))
	      {
		cout << "No data received. Synchronization failed." << endl;
		break; // Receive failed. We try to set up the laser.
	      }
	    int msg_length = 0;
	    if (c[0] == 0x02 && c[1] == 0x80 && c[4] == 0xb0)
	      {
		msg_length += c[2] + 256*c[3] - 1 + 2;
		unsigned char dump[1000];
		cout << "Telegram detected with length " << msg_length << 
		    ". Waited for " << j << " characters." << endl;
		if (msg_length >= 1000 || port.receive(msg_length, dump) != msg_length)
		  {
		    cout << "Telegram not fully received. Synchronization failed." << endl;
		    break; // Receive failed. We try to set up the laser.
		  }
		Stop();
		break; // Receive successful. We stop and reset the configuration.
	      }
	  }
	if (j == 1000)
	  {
	    cout << "Data stream received, but no header found. Trying to reset." << endl;
	    Reset();
	  }
}
int ProtLMS::Init(const char device[])
{
	port.init(device);
/*for(int i=0;i<30;i++)
{
	sleep(3);
	int num_read=port.receive(1,telegram_in);
	printf("power received: %d %c\n",num_read,telegram_in[0]);
}
return -1;*/

	char cadena[100];
    
    int baud[]={9600,38400,500000};
  //  int baud[]={9600,38400};
  // int baud[]={9600};
    int i=0;
	int size=sizeof(baud)/sizeof(int);
    for(i=0;i<=size;i++) 
    {
    	int bps;
		if (i == size) // In case a Reset() returns the laser to 9600bps.
			bps = baud[0];
		else
			bps = baud[i];
		port.setBaud(bps);
		syncronizeFromPreviousCrash();
    	int pru=LMSType(cadena);
		if(pru!=PROT_LMS_OK)
		{
		 printf("Connection at %d baud failed. Reason: %d\n",bps,pru);
		 }
		else
		{
			printf("Connection at %d succesful\n",bps);
		 break;
		 }
	}
	if(i==size)return PROT_LMS_ERROR;
	
	printf("Change Variant 180,1\n");
	int ret=ChangeVariant(180,1.0f);
	if(ret!=PROT_LMS_OK)
	{
		printf("Error change variant %d\n",ret);
		return ret;
	}
	printf("Change Range 8 m\n");
	ret=ChangeMaxRange(PROT_LMS_MAX_RANGE_8M);
	if(ret!=PROT_LMS_OK)
	{
		printf("Error change maxrange %d\n",ret);
		return ret;
	}

	printf("INitialization OK\n");
	return PROT_LMS_OK;
}
void ProtLMS::Reset()
{
//Build telegram
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x01;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x10;

	check_sum.entero=CreateCRC(telegram_out,5);
	telegram_out[5]=check_sum.car_ent.caracter_low;
	telegram_out[6]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(7,telegram_out);
	
//receive
//Check Acknowledge
//for(int i=0;i<15;i++)
{
//	sleep(2);
	num_read=port.receive(1,telegram_in);
	printf("RESET, received: %d\n",num_read);
}
/*	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MEssage	
	int expected_length=29;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	dos_bytes leng;
	leng.car_ent.caracter_low=telegram_in[2];//Length
	leng.car_ent.caracter_high=telegram_in[3];//Length
	if(telegram_in[4]!=0xBA)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	int i;
	for(i=0;i<leng.entero-2;i++)
	{
		cad[i]=telegram_in[5+i];
	}
	cad[i]=0x00;
	if(telegram_in[4+leng.entero-1]!=0x10)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//STATUS

	check_sum.entero=CreateCRC(telegram_in,4+leng.entero);
	if(telegram_in[4+leng.entero]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[4+leng.entero+1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

//All OK
	return PROT_LMS_OK;*/
}

void ProtLMS::Done()
{
	printf("Closing laser\n");
	if(contin)
		Stop();
	
	for(int i=0;i<10;i++)
	{
		unsigned char dummy[1000];
		int ret=port.receive(1000,dummy);
		if(ret>0)
		{
			printf("FLUSHED %d\n",ret);
			Stop();
		}
	}

	ChangeBaudRate(9600);
	port.close();
}

int ProtLMS::ActivateLaser(bool active)
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x68;
	//dato, encendido
	if(active)
		telegram_out[5]=0x01;
	else 
		telegram_out[5]=0x00;
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
	//send it
	port.send(8,telegram_out);
	//receive
	int expected_length=11;
	num_read=0;
	num_read=port.receive(expected_length,telegram_in);
//	if(num_read==0)return PROT_LMS_ERROR_ANSWER_TIMEOUT;
//	if(telegram_in[0]!=0x15)return PROT_LMS_ERROR_NACK;
//	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;

	return PROT_LMS_OK;
}


int ProtLMS::MeasuredValuesReflect(float* meas_x,float* meas_y,int* reflec,int* num_measures)
{
	int num_part=1;//NUMERO DE PARTICIONES
//	int begin_part[1]={1};
	int end_part[1]={101};
	if(scan_resolution==1.0f)end_part[0]=101;
	if(scan_resolution==0.5f)end_part[0]=200;
	if(scan_resolution==0.25f)end_part[0]=400;

	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=(unsigned char)(3+num_part*4);
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x75;
	//dato, all values
	telegram_out[5]=(unsigned char)num_part;
	telegram_out[6]=0;
	int i;
	for(i=0;i<num_part;i++)
	{
		dos_bytes aux;
		aux.entero=1;
		telegram_out[7+i*4]=aux.car_ent.caracter_low;
		telegram_out[8+i*4]=aux.car_ent.caracter_high;
		aux.entero=end_part[i];
		telegram_out[9+i*4]=aux.car_ent.caracter_low;
		telegram_out[10+i*4]=aux.car_ent.caracter_high;
	}
	check_sum.entero=CreateCRC(telegram_out,7+num_part*4);
	telegram_out[7+num_part*4]=check_sum.car_ent.caracter_low;
	telegram_out[8+num_part*4]=check_sum.car_ent.caracter_high;
	port.send(9+num_part*4,telegram_out);

//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
	num_read=port.receive(4,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=4)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	dos_bytes longitud;
	longitud.car_ent.caracter_low=telegram_in[2];
	longitud.car_ent.caracter_high=telegram_in[3];
	num_read=port.receive(longitud.entero+2,telegram_in+4);
	int num_bytes=longitud.entero+6;
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=longitud.entero+2)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[4]==0x92)return PROT_LMS_ERROR_TELEGRAM_NACK;//COMMAND
	if(telegram_in[4]!=0xf5)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND
	//numero de datos a continuacion
	longitud.car_ent.caracter_low=telegram_in[11];
	longitud.car_ent.caracter_high=telegram_in[12];	
	int longi=longitud.entero&0xc000;
	int units;
	float divisor;
	if (longi==0){units=PROT_LMS_UNIT_CM;divisor=100.0f;}
	if (longi==0x8000){units=PROT_LMS_UNIT_10_CM;divisor=10.0f;}
	if (longi==0x4000){units=PROT_LMS_UNIT_MM;divisor=1000.0f;}//DEFAULT
	*num_measures=longitud.entero&0x07ff;
	int step=1;
	if(*num_measures==101)step=4;
	if(*num_measures==200)step=2;
	if(*num_measures==400)step=1;
	dos_bytes med;
	for(i=0;i<*num_measures;i++)
	{
		med.car_ent.caracter_low=telegram_in[13+i*4];
		med.car_ent.caracter_high=telegram_in[14+i*4];
		meas_x[i]=med.entero*cos_alfa[i*step]/divisor;	
		meas_y[i]=med.entero*sen_alfa[i*step]/divisor;
		med.car_ent.caracter_low=telegram_in[15+i*4];
		med.car_ent.caracter_high=telegram_in[16+i*4];
		reflec[i]=med.entero;
	}

/*	dos_bytes med;
	for(i=0;i<401;i++)
	{
		med.car_ent.caracter_low=telegram_in[13+i*4];
		med.car_ent.caracter_high=telegram_in[14+i*4];
		measures[i]=med.entero;
		med.car_ent.caracter_low=telegram_in[15+i*4];
		med.car_ent.caracter_high=telegram_in[16+i*4];
		reflec[i]=med.entero;
	}
*/
/*	if(telegram_in[num_bytes-3]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,num_bytes-2);
	if(telegram_in[num_bytes-2]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[num_bytes-1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;
*/

	return PROT_LMS_OK;
}

int ProtLMS::ChangeModeReflectPartCont(int num_part,int* begin_part,int* end_part)
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=(unsigned char)(3+num_part*4);
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x20;
	//dato, all values
	telegram_out[5]=0x2B;
	telegram_out[6]=(unsigned char)num_part;
	int i;
	for(i=0;i<num_part;i++)
	{
		dos_bytes aux;
		aux.entero=begin_part[i];
		telegram_out[7+i*4]=aux.car_ent.caracter_low;
		telegram_out[8+i*4]=aux.car_ent.caracter_high;
		aux.entero=end_part[i];
		telegram_out[9+i*4]=aux.car_ent.caracter_low;
		telegram_out[10+i*4]=aux.car_ent.caracter_high;
	}
	check_sum.entero=CreateCRC(telegram_out,7+num_part*4);
	telegram_out[7+num_part*4]=check_sum.car_ent.caracter_low;
	telegram_out[8+num_part*4]=check_sum.car_ent.caracter_high;
	port.send(9+num_part*4,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
//	int expected_length=12+num_measures*2;
	num_read=port.receive(400,telegram_in);
	return PROT_LMS_OK;
}

int ProtLMS::MeasuredValuesAll(float* meas_x,float* meas_y,int* ref,int* num_meas)
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x30;
	//dato, all values
	telegram_out[5]=0x01;
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
	port.send(8,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
	num_read=port.receive(4,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=4)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	//longitud del telegrama en bytes
	dos_bytes longitud;
	longitud.car_ent.caracter_low=telegram_in[2];
	longitud.car_ent.caracter_high=telegram_in[3];
	num_read=port.receive(longitud.entero+2,telegram_in+4);
	int num_bytes=longitud.entero+6;
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=longitud.entero+2)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[4]!=0xb0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND
	//numero de datos a continuacion
	longitud.car_ent.caracter_low=telegram_in[5];
	longitud.car_ent.caracter_high=telegram_in[6];	
	int longi=longitud.entero&0xc000;
	int units;
	float divisor;
	if (longi==0){units=PROT_LMS_UNIT_CM;divisor=100.0f;}
	if (longi==0x8000){units=PROT_LMS_UNIT_10_CM;divisor=10.0f;}
	if (longi==0x4000){units=PROT_LMS_UNIT_MM;divisor=1000.0f;}//DEFAULT
	*num_meas=longitud.entero&0x07ff;
	if(*num_meas!=num_measures)return PROT_LMS_ERROR_DATA_EXPECTED;
	int step=1;
	if(*num_meas==101)step=4;
	if(*num_meas==201)step=2;
	if(*num_meas==401)step=1;
	dos_bytes med;
	int mask;
	if(max_range==PROT_LMS_MAX_RANGE_8M)mask=0x1fff;
	if(max_range==PROT_LMS_MAX_RANGE_16M)mask=0x3fff;
	if(max_range==PROT_LMS_MAX_RANGE_32M)mask=0x7fff;
	for(int i=0;i<*num_meas;i++)
	{	
		med.car_ent.caracter_low=telegram_in[7+i*2];
		med.car_ent.caracter_high=telegram_in[8+i*2];
		ref[i]=med.entero&(~mask);
		med.entero&=mask;
		meas_x[i]=med.entero*cos_alfa[i*step]/divisor;	
		meas_y[i]=med.entero*sen_alfa[i*step]/divisor;	
	}

	if(telegram_in[num_bytes-3]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,num_bytes-2);
	if(telegram_in[num_bytes-2]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[num_bytes-1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	return PROT_LMS_OK;
}

int ProtLMS::MeasuredValuesAll(float* measures,int* num_meas)
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x30;
	//dato, all values
	telegram_out[5]=0x01;
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
	port.send(8,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
	num_read=port.receive(4,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=4)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	//longitud del telegrama en bytes
	dos_bytes longitud;
	longitud.car_ent.caracter_low=telegram_in[2];
	longitud.car_ent.caracter_high=telegram_in[3];
	num_read=port.receive(longitud.entero+2,telegram_in+4);
	int num_bytes=longitud.entero+6;
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=longitud.entero+2)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[4]!=0xb0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND
	//numero de datos a continuacion
	longitud.car_ent.caracter_low=telegram_in[5];
	longitud.car_ent.caracter_high=telegram_in[6];	
	int longi=longitud.entero&0xc000;
	int units;
	float divisor;
	if (longi==0){units=PROT_LMS_UNIT_CM;divisor=100.0f;}
	if (longi==0x8000){units=PROT_LMS_UNIT_10_CM;divisor=10.0f;}
	if (longi==0x4000){units=PROT_LMS_UNIT_MM;divisor=1000.0f;}//DEFAULT
	*num_meas=longitud.entero&0x07ff;
	if(*num_meas!=num_measures)return PROT_LMS_ERROR_DATA_EXPECTED;
	int step=1;
	if(*num_meas==101)step=4;
	if(*num_meas==201)step=2;
	if(*num_meas==401)step=1;
	dos_bytes med;
	for(int i=0;i<*num_meas;i++)
	{
		
		med.car_ent.caracter_low=telegram_in[7+i*2];
		med.car_ent.caracter_high=telegram_in[8+i*2];
		med.entero&=0x3fff;
		measures[i]=med.entero/divisor;		
	}

	if(telegram_in[num_bytes-3]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,num_bytes-2);
	if(telegram_in[num_bytes-2]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[num_bytes-1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	return PROT_LMS_OK;
}

int ProtLMS::MeasuredValuesAll(int* num_meas,int* meas)
{
	*num_meas=0;
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x30;
	//dato, all values
	telegram_out[5]=0x01;
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
	port.send(8,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
	num_read=port.receive(4,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=4)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	//longitud del telegrama en bytes
	dos_bytes longitud;
	longitud.car_ent.caracter_low=telegram_in[2];
	longitud.car_ent.caracter_high=telegram_in[3];
	//printf("Se quieren leer %d\n",longitud.entero+2);
	num_read=port.receive(longitud.entero+2,telegram_in+4);
	int num_bytes=longitud.entero+6;
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=longitud.entero+2)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[4]!=0xb0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND
	//numero de datos a continuacion
	longitud.car_ent.caracter_low=telegram_in[5];
	longitud.car_ent.caracter_high=telegram_in[6];	
	int longi=longitud.entero&0xc000;
	int units;
	float divisor;
	if (longi==0){units=PROT_LMS_UNIT_CM;divisor=100.0f;}
	if (longi==0x8000){units=PROT_LMS_UNIT_10_CM;divisor=10.0f;}
	if (longi==0x4000){units=PROT_LMS_UNIT_MM;divisor=1000.0f;}//DEFAULT
	*num_meas=longitud.entero&0x07ff;
	if(*num_meas!=num_measures)return PROT_LMS_ERROR_DATA_EXPECTED;
	dos_bytes med;
//	int mask;
//	if(max_range==PROT_LMS_MAX_RANGE_8M)mask=0x1fff;
//	if(max_range==PROT_LMS_MAX_RANGE_16M)mask=0x3fff;
//	if(max_range==PROT_LMS_MAX_RANGE_32M)mask=0x7fff;
	for(int i=0;i<*num_meas;i++)
	{	
		med.car_ent.caracter_low=telegram_in[7+i*2];
		med.car_ent.caracter_high=telegram_in[8+i*2];
	//	ref[i]=med.entero&(~mask);
	//	med.entero&=mask;
		meas[i]=med.entero;	
	}
	if(telegram_in[num_bytes-3]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,num_bytes-2);
	if(telegram_in[num_bytes-2]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[num_bytes-1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	return PROT_LMS_OK;
}
int ProtLMS::ChangeMaxRange(float max_rang)
{
	dos_bytes check_sum;
	if(max_rang!=PROT_LMS_MAX_RANGE_8M
		&&max_rang!=PROT_LMS_MAX_RANGE_16M
		&&max_rang!=PROT_LMS_MAX_RANGE_32M)
	{
		printf("ChangeMaxRange bad parameter\n");
		return PROT_LMS_ERROR_BAD_PARAMETER;
	}
	int ret=ChangeMode(PROT_LMS_MODE_INSTALLATION);
	if(ret!=PROT_LMS_OK)return ret;
	//Longitud
	telegram_out[2]=0x22;//33 bytes
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x77;
	//datos
	telegram_out[5]=0x00;//Blanking default
	telegram_out[6]=0x00;//Blanking default
	telegram_out[7]=0x46;//70mV
	telegram_out[8]=0x00;//70mV
	telegram_out[9]=0x00;
	if(max_rang==PROT_LMS_MAX_RANGE_8M)
		telegram_out[10]=(unsigned char)1;
	if(max_rang==PROT_LMS_MAX_RANGE_16M)
		telegram_out[10]=(unsigned char)3;
	if(max_rang==PROT_LMS_MAX_RANGE_32M)
		telegram_out[10]=(unsigned char)5;
	telegram_out[11]=1;//units mm
	telegram_out[12]=0;//default temporary
	telegram_out[13]=0;//default field
	telegram_out[14]=2;//default multiple
	for(int i=0;i<23;i++)
		telegram_out[15+i]=0;//contour A		
	check_sum.entero=CreateCRC(telegram_out,38);
	telegram_out[38]=check_sum.car_ent.caracter_low;
	telegram_out[39]=check_sum.car_ent.caracter_high;
	port.send(40,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MESSAGE
//	int expected_length=12+num_measures*2;
	num_read=port.receive(400,telegram_in);

	ret=ChangeMode(PROT_LMS_MODE_SCAN_ON_REQUEST);
	if(ret!=PROT_LMS_OK)return ret;
	max_range=max_rang;
	return PROT_LMS_OK;
}

int ProtLMS::ChangeBaudRate(int baud)
{
	if(baud!=9600 && baud!=38400 && baud!=500000)
		return PROT_LMS_ERROR_BAD_PARAMETER;
		
//Build telegram
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x20;
	//DATO
	if(baud==9600)telegram_out[5]=0x42;
	if(baud==38400)telegram_out[5]=0x40;
	if(baud==500000)telegram_out[5]=0x48;
	
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(8,telegram_out);
//receive
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive message
	int expected_length=9;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	if(telegram_in[2]!=0x03)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xA0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//DATA
	if(telegram_in[6]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,7);
	if(telegram_in[7]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[8]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;
//CHANGE PORT BAUD RATE
	
	if(port.setBaud(baud)!=PORT_OK)
		return PROT_LMS_ERROR;

//All OK
	return PROT_LMS_OK;
}

int ProtLMS::ChangeVariant(int scan_ang,float scan_res)
{
	if(scan_ang!=100&&scan_ang!=180)return PROT_LMS_ERROR_BAD_PARAMETER;
	if(scan_res!=1.0f&&scan_res!=0.5f&&scan_res!=0.25f)return PROT_LMS_ERROR_BAD_PARAMETER;
//Build telegram	
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x05;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x3B;
	//DATOS
	check_sum.entero=scan_ang;
	telegram_out[5]=check_sum.car_ent.caracter_low;
	telegram_out[6]=check_sum.car_ent.caracter_high;
	check_sum.entero=(int)(scan_res*100);
	telegram_out[7]=check_sum.car_ent.caracter_low;
	telegram_out[8]=check_sum.car_ent.caracter_high;

	check_sum.entero=CreateCRC(telegram_out,9);
	telegram_out[9]=check_sum.car_ent.caracter_low;
	telegram_out[10]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(11,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive Message
	int expected_length=13;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	if(telegram_in[2]!=0x07)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xBB)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x01)return PROT_LMS_ERROR_OPERATION_ABORTED;//NOT ACCEPTED
	dos_bytes s_a,s_r;
	s_a.car_ent.caracter_low=telegram_in[6];
	s_a.car_ent.caracter_high=telegram_in[7];
	s_r.car_ent.caracter_low=telegram_in[8];
	s_r.car_ent.caracter_high=telegram_in[9];
	if(telegram_in[10]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS

	check_sum.entero=CreateCRC(telegram_in,11);
	if(telegram_in[11]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[12]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;
	
	scan_angle=s_a.entero;
	scan_resolution=((float)s_r.entero)/100.0f;
	num_measures=(int)(scan_angle/scan_resolution)+1;
	return PROT_LMS_OK;
}

int ProtLMS::LMSType(char* cad)
{
//Build telegram
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x01;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x3A;

	check_sum.entero=CreateCRC(telegram_out,5);
	telegram_out[5]=check_sum.car_ent.caracter_low;
	telegram_out[6]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(7,telegram_out);
	
//receive
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
//	printf("LMSTYPe, received: %d\n",num_read);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive MEssage	
	int expected_length=29;
	num_read=port.receive(expected_length,telegram_in);

//	printf("LMSTYPe, received: %d\n",num_read);

//	for(int j=0;j<30;j++)
//		printf("%x ",telegram_in[j]);
	printf("\n");
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	dos_bytes leng;
	leng.car_ent.caracter_low=telegram_in[2];//Length
	leng.car_ent.caracter_high=telegram_in[3];//Length
	if(telegram_in[4]!=0xBA)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	int i;
	for(i=0;i<leng.entero-2;i++)
	{
		cad[i]=telegram_in[5+i];
	}
	cad[i]=0x00;
	if(telegram_in[4+leng.entero-1]!=0x10)
	{
		printf("Wrong status: %x\n",telegram_in[4+leng.entero-1]);
		return PROT_LMS_ERROR_TELEGRAM_FORMAT;//STATUS
	}

	check_sum.entero=CreateCRC(telegram_in,4+leng.entero);

	if(telegram_in[4+leng.entero]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[4+leng.entero+1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

//All OK
	return PROT_LMS_OK;
}
//
//
//NO TERMINADA
//
//
int ProtLMS::status(char* cad)
{
//Build telegram	
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x01;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x31;

	check_sum.entero=CreateCRC(telegram_out,5);
	telegram_out[5]=check_sum.car_ent.caracter_low;
	telegram_out[6]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(7,telegram_out);
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive Message
	int expected_length=160;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	dos_bytes leng;
	leng.car_ent.caracter_low=telegram_in[2];//Length
	leng.car_ent.caracter_high=telegram_in[3];//Length
	if(telegram_in[4]!=0xB1)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	int i;
	for(i=0;i<leng.entero-2;i++)
	{
		cad[i]=telegram_in[5+i];
	}
/*	if(telegram_in[2]!=0x07)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xBB)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x01)return PROT_LMS_ERROR_OPERATION_ABORTED;//NOT ACCEPTED
	dos_bytes s_a,s_r;
	s_a.car_ent.caracter_low=telegram_in[6];
	s_a.car_ent.caracter_high=telegram_in[7];
	s_r.car_ent.caracter_low=telegram_in[8];
	s_r.car_ent.caracter_high=telegram_in[9];
	if(telegram_in[10]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS

	check_sum.entero=CreateCRC(telegram_in,11);
	if(telegram_in[11]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[12]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;
	
	scan_angle=s_a.entero;
	scan_resolution=((float)s_r.entero)/100.0f;*/
	return PROT_LMS_OK;
}

int ProtLMS::ChangeMode(int mod)
{
	if(mod==PROT_LMS_MODE_INSTALLATION)
		printf("Changing to mode installation\n");
	else if(mod==PROT_LMS_MODE_SCAN_ON_REQUEST)
		printf("Changing to mode scan on request\n");
	else
	{
		printf("Error change mode, bad parameter\n");
		return PROT_LMS_ERROR_BAD_PARAMETER;
	}
//Build telegram
	dos_bytes check_sum;
	//Longitud
	if(mod==PROT_LMS_MODE_INSTALLATION)
	{
		telegram_out[2]=0x0A;
		telegram_out[3]=0x00;
	}
	else
	{
		telegram_out[2]=0x02;
		telegram_out[3]=0x00;
	}
	//Comando
	telegram_out[4]=0x20;
	//DATO
	telegram_out[5]=(unsigned char)mod;
	int len;
	if(mod==PROT_LMS_MODE_INSTALLATION)
	{
		telegram_out[6]='S';telegram_out[7]='I';telegram_out[8]='C';telegram_out[9]='K';telegram_out[10]='_';telegram_out[11]='L';telegram_out[12]='M';telegram_out[13]='S';
		len=14;
	}
	else len=6;
	//Checksum
	check_sum.entero=CreateCRC(telegram_out,len);
	telegram_out[len]=check_sum.car_ent.caracter_low;
	telegram_out[len+1]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(len+2,telegram_out);
//receive
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive message
	int expected_length=9;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	if(telegram_in[2]!=0x03)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xA0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//DATA
	if(telegram_in[6]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,7);
	if(telegram_in[7]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[8]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;
//All OK
	mode=mod;
	printf("Change mode succesful\n");
	return PROT_LMS_OK;
}
int ProtLMS::Start()
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x20;
	//DATO
	telegram_out[5]=0x24;
	
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(8,telegram_out);
//receive
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive message
	int expected_length=9;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	if(telegram_in[2]!=0x03)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xA0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//DATA
	if(telegram_in[6]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,7);
	if(telegram_in[7]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[8]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	contin=true;
//All OK
	return PROT_LMS_OK;
}
int ProtLMS::Stop()
{
	dos_bytes check_sum;
	//Longitud
	telegram_out[2]=0x02;
	telegram_out[3]=0x00;
	//Comando
	telegram_out[4]=0x20;
	//DATO
	telegram_out[5]=0x25;
	
	check_sum.entero=CreateCRC(telegram_out,6);
	telegram_out[6]=check_sum.car_ent.caracter_low;
	telegram_out[7]=check_sum.car_ent.caracter_high;
//send Telegram
	port.send(8,telegram_out);
//receive
//Check Acknowledge
	num_read=port.receive(1,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(telegram_in[0]==0x15)return PROT_LMS_ERROR_NACK;
	if(telegram_in[0]!=0x6) return PROT_LMS_ERROR_MISS_ACK;
//receive message
	int expected_length=9;
	num_read=port.receive(expected_length,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=expected_length)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	if(telegram_in[2]!=0x03)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_L 3bytes
	if(telegram_in[3]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//LENGTH_H
	if(telegram_in[4]!=0xA0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND+80
	if(telegram_in[5]!=0x00)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//DATA
	if(telegram_in[6]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,7);
	if(telegram_in[7]!=check_sum.car_ent.caracter_low)	return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[8]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	contin=false;
//All OK
	return PROT_LMS_OK;
}

int ProtLMS::readLMSdata(int* num, int* values)
{
	*num=0;
	dos_bytes check_sum;

//receive MESSAGE
	num_read=port.receive(4,telegram_in);
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=4)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[0]!=0x02)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//BEGIN
	if(telegram_in[1]!=0x80)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//ADDRESS+80
	//longitud del telegrama en bytes
	dos_bytes longitud;
	longitud.car_ent.caracter_low=telegram_in[2];
	longitud.car_ent.caracter_high=telegram_in[3];

	//printf("Se quieren leer %d\n",longitud.entero+2);
	num_read=port.receive(longitud.entero+2,telegram_in+4);
	int num_bytes=longitud.entero+6;
	if(num_read<0)return num_read;//PORT ERROR= TIMEOUT OR READFILE
	if(num_read!=longitud.entero+2)return PROT_LMS_ERROR_ANSWER_LENGTH;
	if(telegram_in[4]!=0xb0)return PROT_LMS_ERROR_TELEGRAM_FORMAT;//COMMAND
	//numero de datos a continuacion
	longitud.car_ent.caracter_low=telegram_in[5];
	longitud.car_ent.caracter_high=telegram_in[6];	
	int longi=longitud.entero&0xc000;
	int units;
	float divisor;
	if (longi==0){units=PROT_LMS_UNIT_CM;divisor=100.0f;}
	if (longi==0x8000){units=PROT_LMS_UNIT_10_CM;divisor=10.0f;}
	if (longi==0x4000){units=PROT_LMS_UNIT_MM;divisor=1000.0f;}//DEFAULT
	*num=longitud.entero&0x07ff;
	if(*num!=num_measures)return PROT_LMS_ERROR_DATA_EXPECTED;
	dos_bytes med;
//	int mask;
//	if(max_range==PROT_LMS_MAX_RANGE_8M)mask=0x1fff;
//	if(max_range==PROT_LMS_MAX_RANGE_16M)mask=0x3fff;
//	if(max_range==PROT_LMS_MAX_RANGE_32M)mask=0x7fff;
	for(int i=0;i<*num;i++)
	{	
		med.car_ent.caracter_low=telegram_in[7+i*2];
		med.car_ent.caracter_high=telegram_in[8+i*2];
	//	ref[i]=med.entero&(~mask);
	//	med.entero&=mask;
		values[i]=med.entero;	
	}
	if(telegram_in[num_bytes-3]!=0x10)return PROT_LMS_ERROR_TELEGRAM_STATUS;//STATUS
	check_sum.entero=CreateCRC(telegram_in,num_bytes-2);
	if(telegram_in[num_bytes-2]!=check_sum.car_ent.caracter_low)return PROT_LMS_ERROR_TELEGRAM_CRC;//LENGTH_H
	if(telegram_in[num_bytes-1]!=check_sum.car_ent.caracter_high)return PROT_LMS_ERROR_TELEGRAM_CRC;

	return PROT_LMS_OK;
}


}; //namespace mr
