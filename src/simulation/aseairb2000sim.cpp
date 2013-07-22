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

#include "aseairb2000sim.h"
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
IMPLEMENT_MR_OBJECT(AseaIRB2000Sim)

void AseaIRB2000Sim::writeToStream(Stream& stream)
 {SolidEntity::writeToStream(stream);}
void AseaIRB2000Sim::readFromStream(Stream& stream)
 {SolidEntity::readFromStream(stream);}

void AseaIRB2000Sim::writeToXML(XMLElement* parent)
 {SolidEntity::writeToXML(parent);}
void AseaIRB2000Sim::readFromXML(XMLElement* parent)
 {SolidEntity::readFromXML(parent);}

AseaIRB2000Sim::AseaIRB2000Sim()
{
	name="ASEA IRB 2000";
	//Units basic in SI
	
	for(int i=0;i<6;i++)q_init.push_back(0.15 + i*0.1);

	vector<Vector2D> list;

//Base = link[0]
	ComposedEntity *link=new ComposedEntity;
	link->setName("Base");

	PrismaticPart *auxPrism=new PrismaticPart();
	list.push_back(Vector2D(-0.35,-0.175));
	list.push_back(Vector2D(0,-0.35));
	list.push_back(Vector2D(0.09058667,-0.33807404));
	list.push_back(Vector2D(0.175,-0.30310889));
	list.push_back(Vector2D(0.24748737,-0.24748737));
	list.push_back(Vector2D(0.30310889,-0.175));
	list.push_back(Vector2D(0.33807404,-0.09058667));
	list.push_back(Vector2D(0.35,0));
	list.push_back(Vector2D(0.33807404,0.09058667));
	list.push_back(Vector2D(0.30310889,0.175));
	list.push_back(Vector2D(0.24748737,0.24748737));
	list.push_back(Vector2D(0.175,0.30310889));
	list.push_back(Vector2D(0.09058667,0.33807404));
	list.push_back(Vector2D(0,0.35));
	list.push_back(Vector2D(-0.35,0.175));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.2);
	(*link)+=auxPrism;

	CylindricalPart *auxCyl=new CylindricalPart(0.07,0.18);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativePosition(Vector3D(0,0,0.20));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.01,0.15);
	auxCyl->setColor(0.7,0.7,0.7);
	auxCyl->setRelativePosition(Vector3D(0,0,0.27));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.10,0.07);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativePosition(Vector3D(-0.26,0,0.20));
	(*link)+=auxCyl;

//	(*this)+=link;
	links.push_back(link);

//Joint[0]
	SimpleJoint *auxJoint=new SimpleJoint(PI,-PI,true,0,Z_AXIS,false);
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

	auxCyl=new CylindricalPart(0.05,0.175);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativePosition(Vector3D(0,0,0.28));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart();
	list.push_back(Vector2D(0.175,0.175));
	list.push_back(Vector2D(-0.175,0.175));
	list.push_back(Vector2D(-0.175,-0.175));
	list.push_back(Vector2D(0.175,-0.175));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.10);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativePosition(Vector3D(0,0,0.33));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart();
	list.push_back(Vector2D(0.14862485,-0.05));
	list.push_back(Vector2D(0.16898578,-0.04577619));
	list.push_back(Vector2D(0.18598645,-0.03380184));
	list.push_back(Vector2D(0.19682117,-0.01605313));
	list.push_back(Vector2D(0.19970184,0.00454079));
	list.push_back(Vector2D(0.17462396,0.38646611));
	list.push_back(Vector2D(0.16631117,0.42945727));
	list.push_back(Vector2D(0.14758612,0.46903902));
	list.push_back(Vector2D(0.11962113,0.50273326));
	list.push_back(Vector2D(0.08416701,0.52843049));
	list.push_back(Vector2D(0.04344344,0.54452188));
	list.push_back(Vector2D(0,0.55));
	list.push_back(Vector2D(-0.04344344,0.54452188));
	list.push_back(Vector2D(-0.08416701,0.52843049));
	list.push_back(Vector2D(-0.11962113,0.50273326));
	list.push_back(Vector2D(-0.14758612,0.46903902));
	list.push_back(Vector2D(-0.16631117,0.42945727));
	list.push_back(Vector2D(-0.17462396,0.38646611));
	list.push_back(Vector2D(-0.19970184,0.00454079));
	list.push_back(Vector2D(-0.19682117,-0.01605313));
	list.push_back(Vector2D(-0.18598645,-0.03380184));
	list.push_back(Vector2D(-0.16898578,-0.04577619));
	list.push_back(Vector2D(-0.14862485,-0.05));
	auxPrism->setPolygonalBase(list);
	auxPrism->setHeight(0.15);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativeOrientation(X_AXIS,PI/2);
	auxPrism->setRelativePosition(Vector3D(0,-0.14,0.43));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart();
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.15);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativeOrientation(PI/2,0,PI);
	auxPrism->setRelativePosition(Vector3D(0,0.14,0.43));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.06,0.15);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativeOrientation(X_AXIS,-PI/2);
	auxCyl->setRelativePosition(Vector3D(0,-0.14,0.78));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.06,0.15);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativeOrientation(X_AXIS,PI/2);
	auxCyl->setRelativePosition(Vector3D(0,0.14,0.78));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.16,0.08);
	auxCyl->setColor(0.5,0.5,0.5);
	auxCyl->setRelativeOrientation(X_AXIS,PI/2);
	auxCyl->setRelativePosition(Vector3D(0,0.08,0.78));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.12,0.08);
	auxCyl->setColor(0.2,0.2,0.2);
	auxCyl->setRelativeOrientation(X_AXIS,PI/2);
	auxCyl->setRelativePosition(Vector3D(0.1,0.14,0.53));
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.12,0.08);
	auxCyl->setColor(0.2,0.2,0.2);
	auxCyl->setRelativeOrientation(X_AXIS,-PI/2);
	auxCyl->setRelativePosition(Vector3D(-0.1,-0.14,0.53));
	(*link)+=auxCyl;

	link->LinkTo(joints[0]);
	links.push_back(link);

//Joint[1]
	auxJoint=new SimpleJoint(5*PI/9,-5*PI/9,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0,0,0.78));
	auxJoint->setRelativeOrientation(X_AXIS,-PI/2);
	auxJoint->LinkTo(joints[0]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[1]);
	joints.push_back(auxJoint);
//	joints[1]->setDrawReferenceSystem(true);

//Arm 2 = link[2]
	link=new ComposedEntity;
	link->setName("Link 2");
	auxPrism=new PrismaticPart();
	list.push_back(Vector2D(-0.13,0));
	list.push_back(Vector2D(-0.12232596,-0.04400409));
	list.push_back(Vector2D(-0.10020985,-0.08281296));
	list.push_back(Vector2D(-0.06626274,-0.11184475));
	list.push_back(Vector2D(-0.02449252,-0.12767191));
	list.push_back(Vector2D(0.02016935,-0.12842584));
	list.push_back(Vector2D(0.06244998,-0.11401754));
	list.push_back(Vector2D(0.09735763,-0.08614808));
	list.push_back(Vector2D(0.12077102,-0.04810779));
	list.push_back(Vector2D(0.12992593,-0.00438779));
	list.push_back(Vector2D(0.1237415,0.03985024));
	list.push_back(Vector2D(0.10294787,0.07938347));
	list.push_back(Vector2D(0.07,0.10954451));
	list.push_back(Vector2D(0.07,0.42954451));
	list.push_back(Vector2D(-0.06,0.61));
	list.push_back(Vector2D(-0.11,0.61));
	list.push_back(Vector2D(-0.11,0.54));
	list.push_back(Vector2D(-0.13,0.54));
	auxPrism->setPolygonalBase(list);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.015);
	auxPrism->setRelativeOrientation(Z_AXIS,PI);
	auxPrism->setRelativePosition(Vector3D(0,0,0.055));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart();
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.015);
	auxPrism->setRelativeOrientation(Z_AXIS,-PI);
	auxPrism->setRelativePosition(Vector3D(0,0,-0.07));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart();
	list.push_back(Vector2D(0.02104773,0.65885903));
	list.push_back(Vector2D(0.03889101,0.67078153));
	list.push_back(Vector2D(0.05081351,0.68862482));
	list.push_back(Vector2D(0.055,0.71));
	list.push_back(Vector2D(0.05168309,0.72881111));
	list.push_back(Vector2D(0.04213244,0.74535332));
	list.push_back(Vector2D(0.0275,0.7576314));
	list.push_back(Vector2D(0.00955065,0.76416443));
	list.push_back(Vector2D(-0.00955065,0.76416443));
	list.push_back(Vector2D(-0.0275,0.7576314));
	list.push_back(Vector2D(-0.11,0.54));
	list.push_back(Vector2D(-0.13,0.54));
	list.push_back(Vector2D(-0.13,0));
	list.push_back(Vector2D(-0.12557036,-0.03364648));
	list.push_back(Vector2D(-0.1125833,-0.065));
	list.push_back(Vector2D(-0.09192388,-0.09192388));
	list.push_back(Vector2D(-0.065,-0.1125833));
	list.push_back(Vector2D(-0.03364648,-0.12557036));
	list.push_back(Vector2D(0,-0.13));
	list.push_back(Vector2D(0,-0.11));
	list.push_back(Vector2D(-0.02847009,-0.10625184));
	list.push_back(Vector2D(-0.055,-0.09526279));
	list.push_back(Vector2D(-0.07778175,-0.07778175));
	list.push_back(Vector2D(-0.09526279,-0.055));
	list.push_back(Vector2D(-0.10625184,-0.02847009));
	list.push_back(Vector2D(-0.11,0));
	list.push_back(Vector2D(-0.10625184,0.02847009));
	list.push_back(Vector2D(-0.09526279,0.055));
	list.push_back(Vector2D(-0.07778175,0.07778175));
	list.push_back(Vector2D(-0.055,0.09526279));
	list.push_back(Vector2D(-0.02847009,0.10625184));
	list.push_back(Vector2D(0,0.11));

	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.11);
	auxPrism->setRelativeOrientation(Z_AXIS,-PI);
	auxPrism->setRelativePosition(Vector3D(0,0,-0.055));
	(*link)+=auxPrism;

	link->LinkTo(joints[1]);
	links.push_back(link);

//Joint[2]
	auxJoint=new SimpleJoint(PI/3 , -PI/3,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0,-0.71,0));
	auxJoint->setRelativeOrientation(Z_AXIS,PI/2);
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

	auxCyl=new CylindricalPart(0.2,0.04);
	auxCyl->setColor(0.1,0.1,0.1);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.1));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.06,0));
	list.push_back(Vector2D(0.22,0));
	list.push_back(Vector2D(0.22,0.125));
	list.push_back(Vector2D(-0.14,0.125));
	list.push_back(Vector2D(-0.05217194,-0.02963257));
	list.push_back(Vector2D(-0.04301885,-0.04182557));
	list.push_back(Vector2D(-0.02668275,-0.0537404));
	list.push_back(Vector2D(-0.00731645,-0.05955224));
	list.push_back(Vector2D(0.01288075,-0.05860108));
	list.push_back(Vector2D(0.03161515,-0.05099493));
	list.push_back(Vector2D(0.0467592,-0.03759757));
	list.push_back(Vector2D(0.05659307,-0.01993048));
	auxPrism->setPolygonalBase(list);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.02);
	auxPrism->setRelativeOrientation(PI,PI,0);
	auxPrism->setRelativePosition(Vector3D(0,0,0.07));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart;
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.02);
	auxPrism->setRelativeOrientation(PI,PI,0);
	auxPrism->setRelativePosition(Vector3D(0,0,-0.09));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(.2,0.085);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
	auxCyl->setRelativePosition(Vector3D(0.1,-0.125,0));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.085,-0.035));
	list.push_back(Vector2D(0.085,0.135));
	list.push_back(Vector2D(-0.085,0.135));
	list.push_back(Vector2D(-0.085,-0.035));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setColor(1,0.5,0);
	auxPrism->setHeight(0.46);
	auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
	auxPrism->setRelativePosition(Vector3D(0.24,-0.225,0));
	(*link)+=auxPrism;

	link->LinkTo(joints[2]);
	link->setRelativeOrientation(Z_AXIS,-PI/2);
	links.push_back(link);

//Joint[3]
	auxJoint=new SimpleJoint(10*PI/9 , -10*PI/9,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(-0.125,0,0));
	auxJoint->setRelativeOrientation(X_AXIS,PI/2);
	auxJoint->LinkTo(joints[2]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[3]);
	joints.push_back(auxJoint);
//	joints[3]->setDrawReferenceSystem(true);

//Parallel bar
	q_bar[0]=new SimpleJoint(5*PI/9,-5*PI/9,true,0,Z_AXIS,false);
	q_bar[0]->LinkTo(joints[1]);

	q_bar[1]=new SimpleJoint(2*PI,-2*PI,true,0,Z_AXIS,false);
	q_bar[1]->LinkTo(q_bar[0]);

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0,0.03));
	list.push_back(Vector2D(0,-0.03));
	list.push_back(Vector2D(-0.03,-0.03));
	list.push_back(Vector2D(-0.03,0.03));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.20);
	auxPrism->setColor(0.3,0.3,0.3);
	auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
	auxPrism->LinkTo(q_bar[1]);
	links.push_back(auxPrism);

	q_bar[2]=new SimpleJoint(5*PI/9,-5*PI/9,true,0,Z_AXIS,false);
	q_bar[2]->setRelativePosition(Vector3D(-0.18,0,0));
	q_bar[2]->LinkTo(q_bar[1]);

	auxCyl=new CylindricalPart(0.1,0.015);
	auxCyl->setColor(0.7,0.7,0.7);
	auxCyl->setRelativePosition(Vector3D(0,0,-0.05));
	auxCyl->LinkTo(q_bar[2]);
	links.push_back(auxCyl);

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.03,-0.02));
	list.push_back(Vector2D(0.03,0.78));
	list.push_back(Vector2D(-0.03,0.78));
	list.push_back(Vector2D(-0.03,-0.02));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.03);
	auxPrism->setColor(0.85,0.42,0);
	auxPrism->setRelativeOrientation(Z_AXIS,PI);
	auxPrism->LinkTo(q_bar[2]);
	links.push_back(auxPrism);

	auxCyl=new CylindricalPart(0.2,0.015);
	auxCyl->setColor(0.7,0.7,0.7);
	auxCyl->setRelativePosition(Vector3D(0,-0.74,-0.1));
	auxCyl->LinkTo(q_bar[2]);
	links.push_back(auxCyl);

//Arm 4
	link=new ComposedEntity;
	link->setName("Link 4");

	auxCyl=new CylindricalPart(0.01,0.05);
	auxCyl->setColor(0.5,0.5,0.5);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.29,0.085);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativePosition(Vector3D(0,0,0.01));
	(*link)+=auxCyl;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.09,0.09));
	list.push_back(Vector2D(-0.09,0.09));
	list.push_back(Vector2D(-0.09,-0.09));
	list.push_back(Vector2D(0.09,-0.09));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.1);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativePosition(Vector3D(0,0,0.3));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0,-0.09));
	list.push_back(Vector2D(0.16135002,-0.0589167));
	list.push_back(Vector2D(0.18061288,-0.05160283));
	list.push_back(Vector2D(0.19626554,-0.0382034));
	list.push_back(Vector2D(0.20646208,-0.02029861));
	list.push_back(Vector2D(0.21,0));
	list.push_back(Vector2D(0.20646208,0.02029861));
	list.push_back(Vector2D(0.19626554,0.0382034));
	list.push_back(Vector2D(0.18061288,0.05160283));
	list.push_back(Vector2D(0.16135002,0.0589167));
	list.push_back(Vector2D(0,0.09));
	auxPrism->setPolygonalBase(list);
	auxPrism->setHeight(0.05);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
	auxPrism->setRelativePosition(Vector3D(0.09,0,0.4));
	(*link)+=auxPrism;

	auxPrism=new PrismaticPart;
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.05);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
	auxPrism->setRelativePosition(Vector3D(-0.04,0,0.4));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.2,0.04);
	auxCyl->setColor(0.6,0.6,0.6);
	auxCyl->setRelativePosition(Vector3D(-0.1,0,0.55));
	auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
	(*link)+=auxCyl;

	link->LinkTo(joints[3]);
	link->setRelativePosition(Vector3D(0,0,0.3));
	link->setRelativeOrientation(Z_AXIS,PI/2);
	links.push_back(link);

//Joint[4]
	auxJoint=new SimpleJoint(2*PI/3, -2*PI/3,true,0,Z_AXIS,false);
	auxJoint->setRelativePosition(Vector3D(0,0,0.85));
	auxJoint->setRelativeOrientation(X_AXIS,-PI/2);
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

	auxPrism=new PrismaticPart;
	list.push_back(Vector2D(0.09,0.06));
	list.push_back(Vector2D(0,0.06));
	list.push_back(Vector2D(-0.01552914,0.05795555));
	list.push_back(Vector2D(-0.03,0.05196152));
	list.push_back(Vector2D(-0.04242641,0.04242641));
	list.push_back(Vector2D(-0.05196152,0.03));
	list.push_back(Vector2D(-0.05795555,0.01552914));
	list.push_back(Vector2D(-0.06,0));
	list.push_back(Vector2D(-0.05795555,-0.01552914));
	list.push_back(Vector2D(-0.05196152,-0.03));
	list.push_back(Vector2D(-0.04242641,-0.04242641));
	list.push_back(Vector2D(-0.03,-0.05196152));
	list.push_back(Vector2D(-0.01552914,-0.05795555));
	list.push_back(Vector2D(0,-0.06));
	list.push_back(Vector2D(0.09,-0.06));
	auxPrism->setPolygonalBase(list);
	list.clear();
	auxPrism->setHeight(0.08);
	auxPrism->setColor(1,0.5,0);
	auxPrism->setRelativePosition(Vector3D(0,0,-0.04));
	(*link)+=auxPrism;

	auxCyl=new CylindricalPart(0.02,0.04);
	auxCyl->setColor(1,0.5,0);
	auxCyl->setRelativePosition(Vector3D(0.09,0,0));
	auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
	(*link)+=auxCyl;

	auxCyl=new CylindricalPart(0.02,0.04);
	auxCyl->setColor(0.11,0.1,0.1);
	auxCyl->setRelativePosition(Vector3D(0.11,0,0));
	auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
	(*link)+=auxCyl;

	link->LinkTo(joints[4]);
	link->setRelativeOrientation(Z_AXIS,-PI/2);
	links.push_back(link);

//Joint[5]
	auxJoint=new SimpleJoint(10*PI/9 , -10*PI/9,true,0,Z_AXIS,0);
	auxJoint->setRelativeOrientation(X_AXIS,PI/2);
	auxJoint->LinkTo(joints[4]);

	actuator=new Actuator();
	actuator->linkTo(auxJoint);
	actuators.push_back(actuator);

	auxJoint->setValue(q_init[5]);
	joints.push_back(auxJoint);
//	joints[5]->setDrawReferenceSystem(true);

//Arm 6 = link[6]
	auxCyl=new CylindricalPart(0.02,0.03);
	auxCyl->setName("Link 6");

	auxCyl->setColor(0.75,0.75,0.75);
	auxCyl->setRelativePosition(Vector3D(0,0,0.13));
	auxCyl->LinkTo(joints[5]);
	links.push_back(auxCyl);

//Tcp
	tcp=new Tcp();
	tcp->setName("Tcp");
	tcp->setRelativePosition(Vector3D(0,0,0.15));
	tcp->LinkTo(joints[5]);
	tcp->setDrawReferenceSystem(true);

	getConfigurationOf(q_init,conf);
	
	(*this)+=links[0];

	//Speed joints in rad/seg.
	actuators[0]->setSimulationParameters(23*PI/12);//	115º/seg Moidficated->before was 15º/seg
	actuators[1]->setSimulationParameters(23*PI/36);//	115º/seg
	actuators[2]->setSimulationParameters(23*PI/36);//	115º/seg
	actuators[3]->setSimulationParameters(14*PI/9);//	280º/seg
	actuators[4]->setSimulationParameters(5*PI/3);//	300º/seg
	actuators[5]->setSimulationParameters(5*PI/3);//	300º/seg

//Ficha técnica AseaIrb2000 Max speed
	//Axis 1 150°/s
	//Axis 2 150°/s
	//Axis 3 150°/s
	//Axis 4 360°/s
	//Axis 5 360°/s
	//Axis 6 450°/s

}

void AseaIRB2000Sim::ASEAIRB2000Mechanism()
{
	double v;
	v=joints[2]->getValue();
	q_bar[1]->setValue(v);
	q_bar[2]->setValue(-v);
}

bool AseaIRB2000Sim::inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf)
{
	if(_conf == 0x00)
		_conf=getCurrentConfiguration();

	return ASEAIRB2000inverseKinematics(t06,_q,_conf);
}

bool AseaIRB2000Sim::ASEAIRB2000inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf)
{
//*****************************************
	//Safe current coordinates
	vector<double> q_current;
	for(int i=0;i<(int)joints.size();i++)
		q_current.push_back(joints[i]->getValue());
//*****************************************

	if(_conf == 0x00)
		_conf=getCurrentConfiguration();

	double _f,_e,_w;
	configuration(_conf,_f,_e,_w);

/*	if(_conf != _c){//If configuration changed, return false
		return false;
	}*/

	Vector3D pm = t06.position - t06.getVectorW()*(0.15);

//THETA 0
	double theta0 = -_f*atan2(pm.y,pm.x);

//THETA 2
	double z_prima = pm.z - (0.78);

	double r_prima = sqrt(pm.x*pm.x + pm.y*pm.y);

	double dd = sqrt((0.125)*(0.125) + (0.85)*(0.85));
	double phi = atan((0.85)/(0.125));

	double c2a = (r_prima*r_prima + z_prima*z_prima - (0.71)*(0.71) - dd*dd)/(2*(0.71)*dd);

	if((1 - c2a*c2a) < 0)
		return false;

	double theta2 = atan2((-_e*sqrt(1-c2a*c2a)),(c2a)) + _e*phi;

//THETA 1
	double beta = atan2(z_prima,r_prima);
	double alfa = atan2((-_e*dd*sin(acos(c2a))),-_e*(0.71 + dd*c2a));
	double theta1 = PI/2 - beta + _e*alfa;

//DESACOPLO CINEMATICO
	OrientationMatrix m06,m03,m36;

	Matrix3x3 &aux03=m03;

	m06=t06.orientation;


//Adjust the values of the first three joints depending on configuration

	if( _conf & ASEAIRB_ELBOWDOWN )//Si _conf tiene solamente el bit de ELBOWDOWN
	{
		theta1=-PI-theta1;
		theta2*=(-1);

		if(q_current[0]<0)
			theta0=theta0-PI;
		else
			theta0=theta0+PI;
	}
	if(_conf & ASEAIRB_FACEBACK )//Si _conf tiene solamente el bit FACEBACK
	{
		theta0*=(-1);
	}
	if( (_conf & ASEAIRB_FACEBACK) && (_conf & ASEAIRB_ELBOWDOWN ) )//Si _conf tiene el bit FACEBACK y el bit ELBOWDOWN
	{
		if(q_current[0]<-PI/2)
			theta0=-PI-theta0;
		else
			theta0=PI-theta0;
	}

//Get the first three joints corrected
    setJointValue(0,theta0);
	setJointValue(1,theta1);
	setJointValue(2,theta2);

//Obtained T03
	Transformation3D t03 = joints[3]->getAbsoluteT3D();

//Return after position
	for(int i=0;i<6;i++)
		setJointValue(i,q_current[i]);

//aux03 has the same memory direction that m03 and the same valour that t03.orientaiton
    aux03=t03.orientation;

	m36=(m03.inverted())*m06;

//THETA 3
	double theta3 = atan2(m36[1][2] , m36[0][2]);
//THETA 4
	double theta4 = acos(-_f*m36[2][2]);
//THETA 5
	double theta5 = atan2(m36[2][1] , -m36[2][0]);

//CONFIGURATIONS
	if(_conf == (ASEAIRB_INIT))//Conf: 128
	{
		if(PI/2 < q_current[3])
		{
			theta3 = theta3 + PI;
			theta4 *= (-1);
			if(0 < q_current[5])theta5=theta5+PI;
			else theta5=theta5-PI;
		}
		else if(q_current[3] < -PI/2)
		{
			theta3 = theta3 - PI;
			theta4 *= (-1);
			if(0 < q_current[5])theta5=theta5+PI;
			else theta5=theta5-PI;
		}
		else
		{
			if(PI < q_current[5])theta5 = theta5 + 2*PI;
			else if(q_current[5] < -PI)theta5 = theta5-2*PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_FACEBACK))//Conf: 129
	{
		if(q_current[3] < -PI/2)
		{
			theta3 = theta3 - PI;
			theta4 = theta4 - PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < PI/2)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else // PI/2 < q_current[3]
		{
			theta3=theta3+PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_ELBOWDOWN))//Conf: 130
	{
		if(q_current[3] < -PI/2)
		{
			theta3=theta3-PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < PI/2)
		{
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else // PI/2 < q_current[3]
		{
			theta3=theta3+PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_FACEBACK|ASEAIRB_ELBOWDOWN))//Conf: 131
	{
		if(q_current[3] < -PI/2)
		{
			theta3=theta3-PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < PI/2)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else // PI/2 < q_current[3]
		{
			theta3=theta3+PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_WRISTDOWN))//Conf: 132
	{
		if(q_current[3] < -PI)
		{
			theta3=theta3-2*PI;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI < q_current[3] && q_current[3] < -PI/2)
		{
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < 0)
		{
			theta3=theta3-PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(0 < q_current[3] && q_current[3] < PI/2)
		{
			theta3=theta3+PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(PI/2 < q_current[3] && q_current[3] < PI)
		{
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else// PI < q_current[3]
		{
			theta3=theta3+2*PI;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_FACEBACK|ASEAIRB_WRISTDOWN))//Conf: 133
	{
		if(q_current[3] < -PI)
		{
			theta3=theta2-2*PI;
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI < q_current[3] && q_current[3] < -PI/2)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < 0)
		{
			theta3=theta3-PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(0 < q_current[3] && q_current[3] < PI/2)
		{
			theta3=theta3+PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else
				theta5=theta5+PI;
		}
		else if(PI/2 < q_current[3] && q_current[3] < PI)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else //PI < q_current[3]
		{
			theta3=theta3+2*PI;
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_ELBOWDOWN|ASEAIRB_WRISTDOWN))//Conf: 134
	{
		if(q_current[3] < -PI)
		{
			theta3=theta3-2*PI;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI < q_current[3] && q_current[3] < -PI/2)
		{
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < 0)
		{
			theta3=theta3-PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else // 0 < q_current[5]
				theta5=theta5+PI;
		}
		else if(0 < q_current[3] && q_current[3] < PI/2)
		{
			theta3=theta3+PI;
			theta4=(-1)*theta4;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else // 0 < q_current[5]
				theta5=theta5+PI;
		}
		else if(PI/2 < q_current[3] && q_current[3] < PI)
		{
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else // PI < q_current[3]
		{
			theta3=theta3+2*PI;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
	}
	else if(_conf == (ASEAIRB_INIT|ASEAIRB_FACEBACK|ASEAIRB_ELBOWDOWN|ASEAIRB_WRISTDOWN))//Conf: 135
	{
		if(q_current[3] < -PI)
		{
			theta3=theta3-2*PI;
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI < q_current[3] && q_current[3] < -PI/2)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else if(-PI/2 < q_current[3] && q_current[3] < 0)
		{
			theta3=theta3-PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else if(0<q_current[5])
				theta5=theta5+PI;
		}
		else if(0 < q_current[3] && q_current[3] < PI/2)
		{
			theta3=theta3+PI;
			theta4=theta4-PI;
			if(q_current[5]<0)
				theta5=theta5-PI;
			else if(0<q_current[5])
				theta5=theta5+PI;
		}
		else if(PI/2 < q_current[3] && q_current[3] < PI)
		{
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
		else // PI < q_current[3]
		{
			theta3=theta3+2*PI;
			theta4=PI-theta4;
			if(q_current[5]<-PI)
				theta5=theta5-2*PI;
			else if(PI<q_current[5])
				theta5=theta5+2*PI;
		}
	}
	else//There is not configuration
		return false;

	_q.push_back(theta0);
	_q.push_back(theta1);
	_q.push_back(theta2);
	_q.push_back(theta3);
	_q.push_back(theta4);
	_q.push_back(theta5);
	
	if(!checkJointValues(_q))return false;

	for(int i=0;i<6;i++)
		if(fabs(_q[i])<EPS)
			_q[i]=0;

	return true;
}

bool AseaIRB2000Sim::getConfigurationOf(const vector<double> &_q, unsigned char &_conf)
{
	if(!checkJointValues(_q))return false;

	double _f,_e,_w;

//FACE
	if(-PI/2 < _q[0] && _q[0] < PI/2)
		_f=-1.0;//FACE FORWARD
	else
		_f=1.0;//FACE BACK

//ELBOW
	double dd=sqrt((0.125)*(0.125) + (0.85)*(0.85));
	double alpha=atan(0.125 / 0.85);
	//Proyecciones sobre el eje vertical
	if(-0.71*sin(_q[1]) < (dd*cos(alpha - _q[2] - _q[1])))
		_e=-1.0;//ELBOW UP
	else
		_e=1.0;//ELBOW DOWN

//WRIST
	vector<double> q_aux;
	double v;
	for(int i=0;i<6;i++)
	{
		getJointValue(i,v);
		q_aux.push_back(v);
		setJointValue(i,_q[i]);
	}

	Vector3D v1 = joints[3]->getAbsoluteT3D().getVectorW();
	Vector3D v2 = tcp->getAbsoluteT3D().getVectorW();

	for(int i=0;i<6;i++)setJointValue(i,q_aux[i]);

	if(_q[3] != 0)
	{
		if(0.15*sin(_q[4])*sin(_q[3] + PI/2) > 0)//Se puede quitar el valor num. porque no afecta al resultado, pero si para entenderlo
			_w=-1.0;//WRIST UP
		else
			_w=1.0;//WRIST DOWN
	}
	else
	{
		if(0.15*sin(_q[4]) > 0)//Idem
			_w=-1.0;
		else
			_w=1.0;
	}

	return configuration(_f,_e,_w,_conf);
}

bool AseaIRB2000Sim::configuration(unsigned char _conf, double& _f, double& _e, double& _w)
{
	_f=_e=_w=-1.0;
	if(_conf == ASEAIRB_INIT)return true;
	if(_conf & ASEAIRB_FACEBACK)_f=1.0;
	if(_conf & ASEAIRB_ELBOWDOWN)_e=1.0;
	if(_conf & ASEAIRB_WRISTDOWN)_w=1.0;
	return true;
}

bool AseaIRB2000Sim::configuration(double _f, double _e, double _w, unsigned char &_conf)
{
	_conf = ASEAIRB_INIT;
	if( _f == 1.0)_conf = (_conf|ASEAIRB_FACEBACK);
	if( _e == 1.0)_conf = (_conf|ASEAIRB_ELBOWDOWN);
	if( _w == 1.0)_conf = (_conf|ASEAIRB_WRISTDOWN);
	return true;
}




void AseaIRB2000Sim::simulate(double delta_t)
{
	ASEAIRB2000Mechanism();
	RobotSim::simulate(delta_t);
}

};//Namespace mr
