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

#include "adeptonesim.h"
#include <iostream>


namespace mr
{
IMPLEMENT_MR_OBJECT(AdeptOneSim)
void AdeptOneSim::writeToStream(Stream& stream)
 {SolidEntity::writeToStream(stream);}
void AdeptOneSim::readFromStream(Stream& stream)
 {SolidEntity::readFromStream(stream);}

void AdeptOneSim::writeToXML(XMLElement* parent)
 {SolidEntity::writeToXML(parent);}
void AdeptOneSim::readFromXML(XMLElement* parent)
 {SolidEntity::readFromXML(parent);}

AdeptOneSim::AdeptOneSim()
{
	name="SCARA Adept One Sim";
	
/***
Codo derecho   -> elbow = 1;
Codo izquierdo -> elbow = -1;
***/

//Configuracion inicial

	//Configuracion inicial = ELBOW_RIGHT

//Base =links[0]
	vector<Vector2D> list;

	ComposedEntity *link=new ComposedEntity;
	link->setName("Base");
	PrismaticPart *auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.225,-0.225));
	list.push_back(Vector2D(0.225,0.225));
	list.push_back(Vector2D(-0.475,0.225));
	list.push_back(Vector2D(-0.475,-0.225));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.04);
	auxPrism->setColor(0.445,0.601,0.352);
	(*link)+=auxPrism;

	CylindricalPart *auxCyl=new CylindricalPart(0.22,0.20);
	auxCyl->setColor(0.445,0.601,0.352);
	auxCyl->setRelativePosition(Vector3D(0,0,0.04));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.05,0.16);
	auxCyl->setColor(0.445,0.601,0.352);
	auxCyl->setRelativePosition(Vector3D(0,0,0.22+0.04));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.02,0.1);
	auxCyl->setColor(0.5,0.5,0.5);
	auxCyl->setRelativePosition(Vector3D(0,0,0.05+0.22+0.04));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0,-0.085));
	list.push_back(Vector2D(0,0.085));
	list.push_back(Vector2D(-0.07,0.085));
	list.push_back(Vector2D(-0.07,-0.085));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.22);
	auxPrism->setColor(0.445,0.601,0.352);
	auxPrism->setRelativePosition(Vector3D(-0.17,0,0.04));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0,0.20));
	list.push_back(Vector2D(-0.135,0.20));
	list.push_back(Vector2D(-0.235,0.10));
	list.push_back(Vector2D(-0.235,-0.10));
	list.push_back(Vector2D(-0.135,-0.20));
	list.push_back(Vector2D(0,-0.20));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.22);
	auxPrism->setColor(0.445,0.601,0.352);
	auxPrism->setRelativePosition(Vector3D(-0.24,0,0.04));
	(*link)+=auxPrism;
	
	(*this)+=link;
	links.push_back(link);


//Joint 0
	SimpleJoint *auxJoint=new SimpleJoint(5*PI/6,-5*PI/6,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0,0,0.99));
	auxJoint->LinkTo(links[0]);
	joints.push_back(auxJoint);
//Arm 1
	link=new ComposedEntity;
	link->setName("Link 1");
	auxCyl=new CylindricalPart(0.03,0.12);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.66));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.47,0.08);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.63));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.10,0.10);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.16));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.43358588,-0.08858489));
	list.push_back(Vector2D(0.46492273,-0.07954354));
	list.push_back(Vector2D(0.49089819,-0.05981997));
	list.push_back(Vector2D(0.50802391,-0.03206291));
	list.push_back(Vector2D(0.514,0));
	list.push_back(Vector2D(0.50802391,0.03206291));
	list.push_back(Vector2D(0.49089819,0.05981997));
	list.push_back(Vector2D(0.46492273,0.07954354));
	list.push_back(Vector2D(0.43358588,0.08858489));

	list.push_back(Vector2D(0.01254118,0.12939366));
	list.push_back(Vector2D(-0.02343779,0.12786974));
	list.push_back(Vector2D(-0.05761828,0.11653383));
	list.push_back(Vector2D(-0.08737748,0.09625579));
	list.push_back(Vector2D(-0.11043183,0.06859163));
	list.push_back(Vector2D(-0.12501227,0.03566415));
	list.push_back(Vector2D(-0.13,0));//Radio
	list.push_back(Vector2D(-0.12501227,-0.03566415));
	list.push_back(Vector2D(-0.11043183,-0.06859163));
	list.push_back(Vector2D(-0.08737748,-0.09625579));
	list.push_back(Vector2D(-0.05761828,-0.11653383));
	list.push_back(Vector2D(-0.02343779,-0.12786974));
	list.push_back(Vector2D(0.01254118,-0.12939366));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.12);
	auxPrism->setColor(1,1,1);
	auxPrism->setRelativePosition(Vector3D(0,0,-0.06));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.08,0.10);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0,0,0.06));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.01,0.07);
	auxCyl->setColor(0.5,0.5,0.5);
	auxCyl->setRelativePosition(Vector3D(0.425,0,0.06));
	(*link)+=auxCyl;
	link->LinkTo(joints[0]);
	links.push_back(link);
//joint 1
	auxJoint=new SimpleJoint(49*PI/60,-49*PI/60,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0.425,0,0));
	auxJoint->LinkTo(joints[0]);
	joints.push_back(auxJoint);
//Arm 2
	link=new ComposedEntity;
	link->setName("Link 2");

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.380088,-0.05275521));
	list.push_back(Vector2D(0.39875741,-0.04737705));
	list.push_back(Vector2D(0.41423432,-0.0356324));
	list.push_back(Vector2D(0.42443895,-0.01909949));
	list.push_back(Vector2D(0.428,0));
	list.push_back(Vector2D(0.42443895, 0.01909949));
	list.push_back(Vector2D(0.41423432, 0.0356324));
	list.push_back(Vector2D(0.39875741, 0.04737705));
	list.push_back(Vector2D(0.380088, 0.05275521));

	list.push_back(Vector2D(0.008544,0.08858894));
	list.push_back(Vector2D(-0.01608036,0.08753526));
	list.push_back(Vector2D(-0.03947151,0.07976842));
	list.push_back(Vector2D(-0.05983554,0.06588405));
	list.push_back(Vector2D(-0.07561073,0.04694697));
	list.push_back(Vector2D(-0.08558725,0.02440948));
	list.push_back(Vector2D(-0.089,0));//Radio
	list.push_back(Vector2D(-0.08558725,-0.02440948));
	list.push_back(Vector2D(-0.07561073,-0.04694697));
	list.push_back(Vector2D(-0.05983554,-0.06588405));
	list.push_back(Vector2D(-0.03947151,-0.07976842));
	list.push_back(Vector2D(-0.01608036,-0.08753526));
	list.push_back(Vector2D(0.008544,-0.08858894));

	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.12);
	auxPrism->setColor(1,1,1);
	auxPrism->setRelativePosition(Vector3D(0,0,0.07));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.06,0.089);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0,0,0.19));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.59,0.04);
	auxCyl->setColor(1,1,1);
	auxCyl->setRelativePosition(Vector3D(0.375,0,0));
	(*link)+=auxCyl;
	link->LinkTo(joints[1]);
	links.push_back(link);
//joint 2
	auxJoint=new SimpleJoint(0,-0.5,true,0,Z_AXIS,true);//Prismatica
	auxJoint->setRelativePosition(Vector3D(0.375,0,0));
	auxJoint->LinkTo(joints[1]);
	joints.push_back(auxJoint);
	
//Arm 3


	auxCyl=new CylindricalPart(0.58,0.035);
	auxCyl->setName("Link 3");
	auxCyl->setColor(0.6,0.6,0.6);
	auxCyl->LinkTo(joints[2]);
	links.push_back(auxCyl);

//joint 3
	auxJoint=new SimpleJoint(277*PI/180,-277*PI/180,true,0,Z_AXIS,false);
	auxJoint->LinkTo(joints[2]);
	joints.push_back(auxJoint);
//Arm 4
	auxCyl=new CylindricalPart(0.02,0.05);
	auxCyl->setName("Link 4");
	auxCyl->setColor(0.3,0.3,0.3);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.02));
	auxCyl->LinkTo(joints[3]);
	links.push_back(auxCyl);

//Tcp
	tcp=new Tcp();
	tcp->setRelativePosition(Vector3D(0,0,-0.02));
	tcp->LinkTo(joints[3]);

	(*this)+=links[0];
//initial position
	setJointValue(0,0.56549);
	setJointValue(0,1.5708);
	setJointValue(0,-0.44);
	setJointValue(0,0.75398);

}

bool  AdeptOneSim::ADEPTONEinverseKinematics(double yaw,Vector3D p,vector<double> &_q,unsigned char conf)
{
/***
Funcion especifica
Calcula los valores articulares para una posicion
y orientacion determianda relativas al robot
Contiene toda la matematica de la cinematica inversa del SCARA ADEPT ONE
***/
	double d1=0.99;
	double d4=0.02;
	double a1=0.425;
	double a2=0.375;

	double q[4];
//Comprobar que el yaw esta dentro del rango de la coordenada q[3]
	if((yaw<(-277*PI/180))||((277*PI/180)<yaw))return false;

//1º Comprobar la posicion actual del codo
	

	float elbow;
	if(conf==ELBOW_RIGHT)elbow=1;
	else if(conf==ELBOW_LEFT)elbow=-1;
	else elbow=1;//POR DEFECTO
//Joint 3------------------------------
	q[2]=p.z-d1+d4;

//Joint 1 y 2
	double c2=(p.x*p.x+p.y*p.y-a1*a1-a2*a2)/(2*a1*a2);
	if(1-c2*c2<0)return false;
	double s2=elbow*sqrt(1-c2*c2);
	q[1]=atan2(s2,c2);
	double k1=a1+a2*cos(q[1]);
	double k2=a2*sin(q[1]);
	q[0]=atan2(p.y,p.x)-atan2(k2,k1);

//Joint 4------------------------------
	q[3]=yaw-q[0]-q[1];

//3º Tras calcular las coordenadas del punto al que quiero llegar, volver a
//comprobar si el codo esta cambiado con esas coordenadas. Si lo esta, cambiar
//las coordenadas para que cambie el codo.

	double alfa=atan2(p.y,p.x);
	if(((alfa>q[0])&&(elbow==-1))||((alfa<q[0])&&(elbow==1)))
	{
		cout<<"Es necesario cambiar las coordenadas"<<endl;
		double a=q[0]-alfa;
		q[0]=alfa-a;//q1=2*alfa-q1;
		q[1]*=(-1);//Simplemente se cambia de signo
	}

	if(q[3]>=PI)q[3]-=PI;//-----------------------------------------------------------------------------------------------
	//¿No tendria que ser 2*PI?

//Se deben comprobar los rangos de las coordenadas, para evitar que se devuelvan
// valores fuera de los rangos.
	if((q[0]<(-5*PI/6))||((5*PI/6)<q[0]))return false;
	if((q[1]<(-49*PI/60))||((49*PI/60)<q[1]))return false;
	if((q[2]<(-0.5))||(0<q[2]))return false;
	if((q[3]<(-277*PI/180))||((277*PI/180)<q[3]))return false;

//Asigno los valores al vector de referencia
	_q.push_back(q[0]);
	_q.push_back(q[1]);
	_q.push_back(q[2]);
	_q.push_back(q[3]);

	return true;
}



bool  AdeptOneSim::inverseKinematics(Transformation3D t, vector<double> &_q, unsigned char conf)
{
/***
Funcion generica
Simplemente comprueba la configuracion y se encarga de llamar a la funcion especifica
de la cinematica inversa del robot

Recive una T3D relativa al robot
***/

	if(conf==0x00)conf=getCurrentConfiguration();

	Vector3D aux=t.getVectorU();
	double yaw=atan2(aux.y,aux.x);

	return ADEPTONEinverseKinematics(yaw,t.position,_q,conf);
}





bool  AdeptOneSim::getConfigurationOf(const vector<double> &_q, unsigned char &conf)
{
	
//Compruebo rangos de coordenadas
	if(checkJointValues(_q)==false)return false;

	
//Calculo de la configuracion actual correspondiente
//Se anidan dos if para que la variable *conf tenga una valor de retorno aunque la configuracion sea no valida
	if(_q[1]<0)
		conf=ELBOW_LEFT;
	else 
		conf=ELBOW_RIGHT;
	if(fabs(_q[1])<EPS)
		return false; //punto singular
	return true;
}









};//Namespace mr