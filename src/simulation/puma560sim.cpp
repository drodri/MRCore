/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
 * 
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

#include "puma560sim.h"
#include <iostream>
/*
#include "gl/gltools.h"

#include "base/logger.h"
#include "../../world/world.h"
#include "../../world/cylindricalpart.h"
#include "../../world/tcp.h"
*/

namespace mr
{
IMPLEMENT_MR_OBJECT(Puma560Sim)
void Puma560Sim::writeToStream(Stream& stream)
 {SolidEntity::writeToStream(stream);}
void Puma560Sim::readFromStream(Stream& stream)
 {SolidEntity::readFromStream(stream);}
void Puma560Sim::writeToXML(XMLElement* parent)
{
	SolidEntity::writeToXML(parent);
}
void Puma560Sim::readFromXML(XMLElement* parent)
{
	SolidEntity::readFromXML(parent);
}

Puma560Sim::Puma560Sim()
{
	name="Puma 560";
	//Units in meters and rad.
//	for(int j=0;j<6;j++)q_init.push_back(0.15 + j*0.1);//0.15 + j*0.1
	for(int j=0;j<6;j++)q_init.push_back(0);

	f=0;//setFlash()

	vector<Vector2D> list;

//Base = link[0]
	ComposedEntity *link=new ComposedEntity;
	link->setName("Base");

	CylindricalPart *auxCyl=new CylindricalPart(0.52,0.08);
	auxCyl->setColor(0.6,0.4,0.1);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.04,0.17);
	auxCyl->setColor(0.6,0.4,0.1);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.15,0.05);
	auxCyl->setRelativePosition(Vector3D(-0.12,0,0));
	auxCyl->setColor(0.6,0.4,0.1);
	(*link)+=auxCyl;

	(*this)+=link;
	links.push_back(link);

//Joint[0]
	SimpleJoint *auxJoint=new SimpleJoint(8*PI/9 , -8*PI/9,true,0,Z_AXIS,false);
	auxJoint->LinkTo(links[0]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[0]);
	joints.push_back(auxJoint);
//	joints[0]->setDrawReferenceSystem(true);

//Arm 1 = link[1]
	link=new ComposedEntity;
	link->setName("Link 1");

	auxCyl=new CylindricalPart(0.14,0.1);
	auxCyl->setColor(0.9,0.9,0.9);
	auxCyl->setRelativePosition(Vector3D(0,0,0.52));//0.66 - 0.52= -0.14
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.17,0.1);
	auxCyl->setColor(0.9,0.9,0.9);
	auxCyl->setRelativePosition(Vector3D(0,0,0.66));
	auxCyl->setRelativeOrientation(0,-PI/2,PI);
	(*link)+=auxCyl;

	PrismaticPart *auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0,0.1));
	list.push_back(Vector2D(0.1,0.1));
	list.push_back(Vector2D(0.1,-0.1));
	list.push_back(Vector2D(0,-0.1));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(0.9,0.9,0.9);
	auxPrism->setHeight(0.25);
	auxPrism->setRelativePosition(Vector3D(0,0,0.52));//-0.14
	auxPrism->setRelativeOrientation(Z_AXIS,PI);
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.03,0.035);//Base flash
	auxCyl->setColor(0.9,0.9,0.9);
	auxCyl->setRelativePosition(Vector3D(-0.05,0,0.77));//0.66+0.11
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.04,0.03);//SET_FLASH
	auxCyl->setColor(1,1,0);
	auxCyl->setRelativePosition(Vector3D(-0.05,0,0.785));//0.125 + 0.66
	(*link)+=auxCyl;
	light=auxCyl;//Copy memory direction

	auxCyl=new CylindricalPart(0.025,0.12);//GRIS finito
	auxCyl->setColor(0.7,0.7,0.7);
	auxCyl->setRelativePosition(Vector3D(0.17,0,0.66));
	auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
	(*link)+=auxCyl;

	link->LinkTo(joints[0]);
	links.push_back(link);

//Joint[1]
	auxJoint=new SimpleJoint(25*PI/36 , -25*PI/36,true,0,Z_AXIS,false);//(25*PI/36 , -25*PI/36);
	auxJoint->setRelativePosition(Vector3D(0.15,0,0.66));//Desplazamiento d2
	auxJoint->setRelativeOrientation(PI/2,-PI/2,PI/2);
	auxJoint->LinkTo(joints[0]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[1]);
	joints.push_back(auxJoint);
//	joints[1]->setDrawReferenceSystem(true);

//Arm 2 = link[2]
/***
Este brazo, como es un objeto unico y no la composicion de varios cuerpos, el
link se inicializa como un objeto solido, no como la composicion de varios. Se
podria dejar asi, pero no tiene mucho sentido la composicion de un unico objeto.
***/
	auxPrism=new PrismaticPart;
	auxPrism->setName("Link 2");

	list.push_back(Vector2D(0.15,0));
	list.push_back(Vector2D(0.07,0.432));
	list.push_back(Vector2D(0.06142221,0.45502801));
	list.push_back(Vector2D(0.04552583,0.47376762));
	list.push_back(Vector2D(0.02420495,0.48598596));
	list.push_back(Vector2D(0,0.49022719));
	list.push_back(Vector2D(-0.02420495,0.48598596));
	list.push_back(Vector2D(-0.04552583,0.47376762));
	list.push_back(Vector2D(-0.06142221,0.45502801));
	list.push_back(Vector2D(-0.07,0.432));
	list.push_back(Vector2D(-0.15,0));
	list.push_back(Vector2D(-0.15,-0.12));
	list.push_back(Vector2D(-0.10479903,-0.14998657));
	list.push_back(Vector2D(-0.05387139,-0.16866003));
	list.push_back(Vector2D(0,-0.175));
	list.push_back(Vector2D(0.05387139,-0.16866003));
	list.push_back(Vector2D(0.10479903,-0.14998657));
	list.push_back(Vector2D(0.15,-0.12));

	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,1,1);
	auxPrism->setHeight(0.1);
	auxPrism->setRelativePosition(Vector3D(0,0,0.045));//0.195 - 0.15
	auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
	//(*link)+=auxPrism;
	auxPrism->LinkTo(joints[1]);
	links.push_back(auxPrism);

//Joint[2]
	auxJoint=new SimpleJoint(3*PI/4,-3*PI/4,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0.432,0,0));//Desplazamiento a2
	auxJoint->LinkTo(joints[1]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[2]);
	joints.push_back(auxJoint);
//	joints[2]->setDrawReferenceSystem(true);

//Arm 3 = link[3]
	link=new ComposedEntity;
	link->setName("Link 3");

	auxCyl=new CylindricalPart(0.15,0.04);
	auxCyl->setColor(0.7,0.7,0.7);
	auxCyl->setRelativePosition(Vector3D(0,0,0.005));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.08159934,-0.03039292));
	list.push_back(Vector2D(0.045,0.36));
	list.push_back(Vector2D(-0.045,0.36));
	list.push_back(Vector2D(-0.08159934,-0.03039292));
	list.push_back(Vector2D(-0.08057515,-0.05303013));
	list.push_back(Vector2D(-0.07339124,-0.07452161));
	list.push_back(Vector2D(-0.06059678,-0.0932244));
	list.push_back(Vector2D(-0.04316989,-0.10770873));
	list.push_back(Vector2D(-0.02244279,-0.11686731));
	list.push_back(Vector2D(0,-0.12));
	list.push_back(Vector2D(0.02244279,-0.11686731));
	list.push_back(Vector2D(0.04316989,-0.10770873));
	list.push_back(Vector2D(0.06059678,-0.0932244));
	list.push_back(Vector2D(0.07339124,-0.07452161));
	list.push_back(Vector2D(0.08057515,-0.05303013));

	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(0.8,0.8,0.8);
	auxPrism->setHeight(0.09);
	auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
	auxPrism->setRelativePosition(Vector3D(0,-0.02,-0.045));//Desplazamiento a3
//y Desplazamiento de la mitad de la altura del prisma/brazo para que el "eje"
//vaya por el centro
	(*link)+=auxPrism;
	link->LinkTo(joints[2]);
	links.push_back(link);

//Joint[3]
	auxJoint=new SimpleJoint(7*PI/9 , -7*PI/9,true,0,Z_AXIS,0);
	auxJoint->setRelativePosition(Vector3D(0.36,-0.02,0));
	auxJoint->setRelativeOrientation(PI,-PI/2,0);
	auxJoint->LinkTo(joints[2]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[3]);
	joints.push_back(auxJoint);
//	joints[3]->setDrawReferenceSystem(true);

//Arm 4 = link[4]
	link=new ComposedEntity;
	link->setName("Link 4");
	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.045,-0.045));
	list.push_back(Vector2D(0.045,0.045));
	list.push_back(Vector2D(-0.045,0.045));
	list.push_back(Vector2D(-0.045,-0.045));

	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(0.39,0.39,0.39);
	auxPrism->setHeight(0.027);
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.035,0.045);
	auxCyl->setColor(0.39,0.39,0.39);
	auxCyl->setRelativePosition(Vector3D(0.01,0,0.072));//0.072 Desplazamiento
//que falta desde 0.36 hasta los 0.432
	auxCyl->setRelativeOrientation(PI/2,0,PI/2);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.035,0.045);
	auxCyl->setColor(0.39,0.39,0.39);
	auxCyl->setRelativePosition(Vector3D(-0.01,0,0.072));//IDEM
	auxCyl->setRelativeOrientation(-PI/2,0,PI/2);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.1,0.02);
	auxCyl->setColor(0.69,0.69,0.69);
	auxCyl->setRelativePosition(Vector3D(-0.05,0,0.072));
	auxCyl->setRelativeOrientation(PI/2,0,PI/2);
	(*link)+=auxCyl;

	link->LinkTo(joints[3]);
	links.push_back(link);

//Joint[4]
	auxJoint=new SimpleJoint(5*PI/9,-5*PI/9,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0,0,0.072));//Desplazamiento desde
//0.36 hasta los 0.432
	auxJoint->setRelativeOrientation(PI,-PI/2,0);
	auxJoint->LinkTo(joints[3]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[4]);
	joints.push_back(auxJoint);
//	joints[4]->setDrawReferenceSystem(true);

//Arm 5 = link[5]
	link=new ComposedEntity;
	link->setName("Link 5");
	auxCyl=new CylindricalPart(0.02,0.045);
	auxCyl->setColor(0.55,0.55,0.55);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.01));//Desplazamiento para que
//coincida en la mitad del cilindro con el eje
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.01,0.022);
	auxCyl->setColor(0.55,0.55,0.55);
	auxCyl->setRelativePosition(Vector3D(0.045,0,0));
	auxCyl->setRelativeOrientation(0,PI/2,0);
	(*link)+=auxCyl;

	link->LinkTo(joints[4]);
	links.push_back(link);

//Joint[5]
	auxJoint=new SimpleJoint(133*PI/90,-133*PI/90,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0.055,0,0));
	auxJoint->setRelativeOrientation(PI/2,0,PI/2);
	auxJoint->LinkTo(joints[4]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[5]);
	joints.push_back(auxJoint);
//	joints[5]->setDrawReferenceSystem(true);

//Arm 6 = link[6]
	//link=new ComposedEntity;
	//link->setName("Link 6");
	auxCyl=new CylindricalPart(0.01,0.025);
	auxCyl->setName("Link 6");
	auxCyl->setColor(0.75,0.75,0.75);
	//(*link)+=auxCyl;
	auxCyl->LinkTo(joints[5]);
	links.push_back(auxCyl);

//Tcp
	tcp=new Tcp();
	tcp->setName("Tcp");
	tcp->setRelativePosition(Vector3D(0,0,0.01));
	tcp->LinkTo(joints[5]);
	tcp->setDrawReferenceSystem(true);

	getConfigurationOf(q_init,conf);



	actuators[0]->setSimulationParameters(23*PI/12);//	115º/seg Moidficated->before was 15º/seg
	actuators[1]->setSimulationParameters(23*PI/36);//	115º/seg
	actuators[2]->setSimulationParameters(23*PI/36);//	115º/seg
	actuators[3]->setSimulationParameters(14*PI/9);//	280º/seg
	actuators[4]->setSimulationParameters(5*PI/3);//	300º/seg
	actuators[5]->setSimulationParameters(5*PI/3);//	300º/seg


	(*this)+=links[0];
}

bool Puma560Sim::getCoordinatesOf(vector<double> &_q)
{
	double v;
	_q.clear();
	for(int i=0;i<6;i++)
	{
		getJointValue(i,v);
		_q.push_back(v);
	}
	return true;
}

bool Puma560Sim::inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf)
{
	if(_conf == 0x00)
		_conf=getCurrentConfiguration();

	return PUMA560inverseKinematics(t06,_q,_conf);
}

bool Puma560Sim::PUMA560inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf)
{
//*****************************************
	vector<double> q_act;
	unsigned char c_act;
	getCoordinatesOf(q_act);//Save current coordiantes
	getConfigurationOf(q_act,c_act);
//*****************************************

	double d1=0.66;
	double d2=0.15;
	double d4=0.432;
	double d6=0.065;
	double a2=0.432;
	double a3=-0.02;//¿?

	if(_conf == 0x00)
		_conf=getCurrentConfiguration();

	double _s, _e, _w;
	configuration(_conf,_s,_e,_w);

/*	if(_conf != c_act)//Si cambia de configuracion, retorna
		return false;
	*/

	Vector3D pm = t06.position - t06.getVectorW()*(d6);

	double z_prima = pm.z - d1;

	double ee = sqrt(pm.x*pm.x + pm.y*pm.y - (d2)*(d2));

	double dd = sqrt((a3)*(a3) + (a2)*(a2));

	double alfa = atan(abs(a3)/a2);

//THETA 2
	double c2a = (ee*ee + z_prima*z_prima - (a2)*(a2) - dd*dd)/(2*(a2)*dd);
	if((1 - c2a*c2a) < 0)
		return false;
	//double theta2 = PI/2 - atan2((_e*sqrt(1 - c2a*c2a)),(c2a)) + alfa;
	double theta2 =  - atan2((_e*sqrt(1 - c2a*c2a)),(c2a)) + alfa;

//THETA 1
	double beta = atan2(z_prima,ee);
	double phi = atan2(dd*sqrt(1 - c2a*c2a),((d4)+dd*c2a));
	double theta1 = -beta + _e*phi + PI/2;

//THETA 0
	double a11=atan2(pm.y,pm.x);
	double a22=atan2(ee,d2);
	double theta0 = a11 - _s*a22;
	if(pm.x < 0 && pm.y > 0 && q_act[0] < -PI/2)
		theta0-=2*PI;

//DESACOPLO CINEMATICO
	OrientationMatrix m06,m03,m36;

	Matrix3x3 &aux03=m03;

	m06=t06.orientation;

//Definicion de T03

/***
Se definen previamente las operaciones de senos y cosenos para que no se repita
la misma operacion varias veces ya que estas operaciones requieren mucho computo.
***/


//Meto las 3 primeras coordenadas finales y corregidas
	setJointValue(0,theta0);
	setJointValue(1,theta1);
	setJointValue(2,theta2);

//Obtengo la informacion que necesito
	Transformation3D auxm = joints[3]->getAbsoluteT3D();

	//Devuelvo la posicion previa
	for(int i=0;i<6;i++)
		setJointValue(i,q_act[i]);

//aux03 tiene la misma direcicon de memoria que m03
	aux03=auxm.orientation;
	
	m36=(m03.inverted())*m06;

//THETA 3
	double theta3 = atan2(m36[1][2] , m36[0][2]) + PI/2;
//THETA 4
	double theta4 = acos(m36[2][2]);
//THETA 5
	double theta5 = atan2(m36[2][1] , -m36[2][0]);

//CONFIGURACIONES

	_q.push_back(theta0);
	_q.push_back(theta1);
	_q.push_back(theta2);
	_q.push_back(theta3);
	_q.push_back(theta4);
	_q.push_back(theta5);

/*	if(!checkJointValues(_q))
		return false;
*/

	for(int i=0;i<6;i++){if(fabs(_q[i])<EPS){_q[i]=0;}}

	return true;
}

bool Puma560Sim::getConfigurationOf(const vector<double> &_q, unsigned char &_conf)
{
//Compruebo tamaño y rangos de coordenadas
	if(!checkJointValues(_q))return false;

	double _s,_e,_w;

//HOMBRO
	if(_q[1]>0)//Hombro izquierdo
		_s=-1.0;
	else//Hombro derecho
		_s=1.0;

//CODO
	double d1=0.02*cos(_q[1] + _q[2]);
	double d2=0.432*sin(_q[1] + _q[2]);
	double d3=0.432*sin(_q[1]);

	if(0 < (d2-d1-d3))
		_e=-1.0;
	else
		_e=1.0;

//WRIST
	vector<double> q_aux;
	getCoordinatesOf(q_aux);
	for(int i=0;i<6;i++)
		joints[i]->setValue(_q[i]);
	Vector3D v1=joints[3]->getAbsoluteT3D().getVectorW();
	Vector3D v2=tcp->getAbsoluteT3D().getVectorW();
	for(int i=0;i<6;i++)
		joints[i]->setValue(q_aux[i]);

	if(_q[3] != 0)
	{
		if(sin(_q[4])*sin(_q[3] + PI/2) > 0)
			_w=-1.0;
		else
			_w=1.0;//WRIST DOWN
	}
	else
	{
		if(sin(_q[4]) > 0)
			_w=-1.0;
		else
			_w=1.0;//WRIST DOWN
	}

	return configuration(_s,_e,_w,_conf);
}

bool Puma560Sim::Pruebas()
{
/*	Transformation3D t06=tcp->getAbsoluteT3D();

	cout<<t06.orientation<<endl<<endl;

	cout<<t06.position<<endl<<endl;*/
	
	return true;
}

bool Puma560Sim::configuration(unsigned char _conf, double& _s, double& _e, double& _w)
{
	_s=_e=_w=-1.0;
	if(_conf == INIT_PUMA)return true;
	if(_conf & SHOULDERRIGHT)_s=1.0;
	if(_conf & ELBOWDOWN)_e=1.0;
	if(_conf & WRISTDOWN)_w=1.0;
	return true;
}

bool Puma560Sim::configuration(double _s, double _e, double _w, unsigned char &_conf)
{
	_conf = INIT_PUMA;
	if( _s == 1.0)_conf = (_conf|SHOULDERRIGHT);
	if( _e == 1.0)_conf = (_conf|ELBOWDOWN);
	if( _w == 1.0)_conf = (_conf|WRISTDOWN);
	return true;
}

void Puma560Sim::setFlash()
{
	if(f == 3)
	{
		light->setColor(1,1,0);
		f=0;
	}
	else
	{
		f+=1;
		light->setColor(0.5,0.5,0);
	}
}


void Puma560Sim::simulate(double delta_t)
{
	setFlash();
	RobotSim::simulate(delta_t);

}

};//Namespace mr
