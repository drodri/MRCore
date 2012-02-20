#include "powercube70hw.h"
#include <iostream>

// Metodos de la clase PowerCube70
namespace mr
{
PowerCube70hw::PowerCube70hw()
{
	Tilt.SetID(13);
	Pan.SetID(14);
	Tilt.SetComPort(&port);
	Pan.SetComPort(&port);
//	pthread_mutex_init(&mutex,NULL);
//	new_pos=0;
}

PowerCube70hw::~PowerCube70hw()
{
	Tilt.Stop();
	Pan.Stop();
	port.close();
}

int PowerCube70hw::init(const char device[])
{		
	std::cout<<"Init powercube"<<std::endl;
//serial port INit
	port.init(device);
	if(port.setBaud(38400)!=PORT_OK)
	{	
		std::cout<<"Error al inicializar puerto"<<std::endl;
		return ERROR_PORT;
	}

	int r;
	r=Reset();
	if(r!=NOERROR)return r;
	r=Home();
	if(r!=NOERROR)return r;
	std::cout<<"Homing..."<<std::endl;
#ifdef WIN32
	Sleep(4);
#else
	sleep(4);
#endif
	std::cout<<"Homed..."<<std::endl;
	SetSpeed(0.5,0.5);
	SetAccel(0.5,0.5);
	////Calibrado de Posicion a Tilt=0, Pan=0
	//Sleep?En el caso de que la posicion diste mucho del homing por PW.Init
	return NOERROR;
}

int PowerCube70hw::Reset()
{
	int r;
	
	r=Pan.Reset();
	std::cout<<"Reset pan: "<<r<<std::endl;
	if(r!=NOERROR)return r;
	
	r= Tilt.Reset();
	std::cout<<"Reset tilt: "<<r<<std::endl;
	if(r!=NOERROR)return r;

	return NOERROR;
}

int PowerCube70hw::Home()
{
	int r;
	
	r= Tilt.Home();
	std::cout<<"Home tilt: "<<r<<std::endl;
	if(r!=NOERROR)return r;
	r= Pan.Home();
	std::cout<<"Home pan: "<<r<<std::endl;
	if(r!=NOERROR)return r;

	return NOERROR;
}

int PowerCube70hw::SetAccel(float acc_pan,float acc_tilt)
{
	int retorno1,retorno2=NOERROR;
	
	retorno1= Tilt.SetAccel(acc_tilt);
	retorno2= Pan.SetAccel(acc_pan);
	
	if(retorno1!=NOERROR || retorno2!=NOERROR)
	{
		std::cout<<"Error en el SetAccel"<<std::endl;
		return 0;
	}
	return(1);
}

int PowerCube70hw::SetSpeed(float vel_pan,float vel_tilt)
{
	int retorno1,retorno2=NOERROR;
	
	retorno1= Tilt.SetSpeed(vel_tilt);
	retorno2= Pan.SetSpeed(vel_pan);
	
	if(retorno1!=0 || retorno2!=0)
	{
		std::cout<<"Error en SetSpeed"<<std::endl;
		return 0;
	}
	return(1);
}

/*float PowerCube70hw::Error(unsigned int Axes, float Pos)
{	
	if(Axes==1 || Axes==0){
		printf("Error = %f\n",fabs(Pos-(Tilt.Pos())));
		return(fabs(Pos-(Tilt.Pos())));}
	if(Axes==2)
		return(fabs(Pos-(Pan.Pos())));	
}*/

int PowerCube70hw::MovePan(float pos)
{
	if(pos>=-0.1 && pos<=0.1)
		return Pan.Move(pos);
	return -1;
}

int PowerCube70hw::MoveTilt(float pos)
{
	if(pos>=-0.7 && pos<=0.7)
		return Tilt.Move(pos);
	return -1;
}

int PowerCube70hw::Move(float pos_pan, float pos_tilt)
{
//	Pan.SetSpeed(0.1F);
//	Tilt.SetSpeed(0.05F);
//	Pan.SetAccel(0.1F);
//	Pan.SetAccel(0.1F);
	int r1=NOERROR;
	r1=Pan.Move(pos_pan);
	std::cout<<"Move Pan: "<<r1<<std::endl;
	int r2=Tilt.Move(pos_tilt);
	std::cout<<"Move Tilt: "<<r2<<std::endl;
	if(r1<r2)
		return r1;
	return r2;

}
/*
int PowerCube70hw::Mov(unsigned int Axes,float Pos,unsigned int FinalPos)
{
	
//struct timezone tz;
//struct timeval ahora, luego;	
	int retorno=0,n=0;
	std::cout<<"En Movimiento....."<<std::endl;
	if(Axes==1 || Axes==0)
		{	
			retorno=Tilt.Move(Pos);
			if(Axes==0)
			retorno=retorno+Pan.Move(Pos);

			//Esto me parece que hay que cambiarlo, la funcion Move no puede
			//dejarte en un while hasta que llegue
			while(Error(Axes,Pos)>=0.01)
			{
				
				//pthread_mutex_trylock(&mutex);
				//gettimeofday(&ahora,&tz); 
				//cout<<n<<"Posicion Tilt: "<<PW.Tilt.Pos()<<"º	Pan: "<<PW.Pan.Pos()<<"º"<<endl;
				if(FinalPos)
					{			
						buffer[0]=Tilt.Pos();
						buffer[1]=Pan.Pos(); 
						//pthread_mutex_unlock(&mutex);
						//Z=1;
					}


			}
			//pthread_mutex_trylock(&mutex);
		
			
		}
	if(Axes==2)
		{	
			retorno=retorno+Pan.Mov(Pos);	
			while(Error(Axes,Pos)>=0.01)
			{	n++;
				pthread_mutex_trylock(&mutex);
				cout<<n<<"Posicion Pan:        "<<Pan.Pos()<<endl;
				if(FinalPos)
					{
					
					}
			}
			pthread_mutex_trylock(&mutex); //para bloqueo por parte del laser	
		}
	if(retorno!=OK)
		{
			cout<<"Error en el Mov del main"<<endl;
			return -1;
		}
	return(OK);
	
	}*/

/*void PowerCube70hw::Zero()
{	
	Accel(60.0, 60.0);
	Speed(10.0, 10.0);
	Move(0,0);
	//cout<<"Muñeca calibrada. Tilt=0, Pan=0"<<endl;
}*/

/*int PowerCube70hw::UpdatePos()
{
	float p,t;
		
	Pan.GetPos(p);
	Tilt.GetPos(t);
	//printf("Pan %f Tilt %f\n", p, t);
//	pthread_mutex_lock(&mutex);
	current_pan=p;
	current_tilt=t;
	new_pos=1;
//	pthread_mutex_unlock(&mutex);

	return new_pos;

}*/
int PowerCube70hw::GetPan(float& p)
{
	int ret=Pan.GetPos(p);
	return ret;
}
int PowerCube70hw::GetTilt(float& t)
{	
	int ret=Tilt.GetPos(t);
	if(ret!=NOERROR)
		std::cout<<"Error GetTilt: "<<ret<<std::endl;
	return ret;
}
int PowerCube70hw::GetPos(float& pan_angle, float& tilt_angle)
{	
//	pthread_mutex_lock(&mutex);
	
	int r1=Pan.GetPos(pan_angle);
	int r2=Tilt.GetPos(tilt_angle);
	
	if(r1<r2)
		return r1;
	return r2;
	
//	pthread_mutex_unlock(&mutex);
	
//	return ret;
}
}; //Namespace mr