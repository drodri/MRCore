
//Clases a incluir
#ifndef __POWERCUBE70HW_H_
#define __POWERCUBE70HW_H_

#include "../powercube70.h"
#include "powercubeaxis.h"
//#include <pthread.h>

namespace mr
{
	class PowerCube70hw : public PowerCube70			
{

public:
	PowerCube70hw();
	~PowerCube70hw();

	int init(const char device[]);
	void close(){port.close();}

	//command functions 0 if error, 1 if success
	int Reset();
	int Home();
	int SetAccel(float acc_pan,float acc_tilt);
	int SetSpeed(float vel_pan,float vel_tilt);
	int Move(float pos_pan,float pos_tilt);
	int MovePan(float pos_pan);
	int MoveTilt(float pos_tilt);///Positive tilt is laser down. Limits: +-0.7 (rad)
	int GetPan(float& pan);
	int GetTilt(float& tilt);
	int GetPos(float& pan_angle,float& tilt_angle);

	//int Mov(unsigned int Axes,float Pos,unsigned int FinalPos);
	//void Zero();
	//pthread_mutex_t mutex; 

protected:
	SerialPort port;
	PowerCubeAxis	Tilt;
	PowerCubeAxis	Pan;

//	float current_pan;
//	float current_tilt;

//	int new_pos;


};
}; //Namespace mr

#endif
