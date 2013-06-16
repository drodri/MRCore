#include "EuitiBotSim.h"
#include <iostream>



namespace mr
{
		IMPLEMENT_MR_OBJECT(EuitiBotSim)

		void EuitiBotSim::writeToStream(Stream& stream)
		{SolidEntity::writeToStream(stream);}
		void EuitiBotSim::readFromStream(Stream& stream)
		{SolidEntity::readFromStream(stream);}
		void EuitiBotSim::writeToXML(XMLElement* parent){
		SolidEntity::writeToXML(parent);
		}
		void EuitiBotSim::readFromXML(XMLElement* parent){
		SolidEntity::readFromXML(parent);
		}

		EuitiBotSim::EuitiBotSim(void)
		{
			name = "EuitiBot";
			for(int j=0;j<6;j++)q_init.push_back(0);

			vector<Vector2D> list;

			//// +++++++++++++++++++++++++++++ ////
			//// +++++++++++++++++++++++++++++ ////
			//// COMENZAMOS A DIBUJAR EL ROBOT ////
			//// +++++++++++++++++++++++++++++ ////
			//// +++++++++++++++++++++++++++++ ////


///////////////////////////////////////////////////////////////////
////////////////////////// BASE = LINK[0]//////////////////////////
///////////////////////////////////////////////////////////////////

			//NUEVO GRUPO DE PIEZAS
			ComposedEntity *link=new ComposedEntity;
			link->setName("Base");

//**************************************//
// LA BASE ESTA FORMADA POR UN CILINDRO //
//**************************************//

			// CILINDRO(Cilindro)
			CylindricalPart *auxCyl=new CylindricalPart(0.5,0.425);//Altura y radio
			auxCyl->setColor(255,233,0);
			(*link)+=auxCyl;

			// AÑADIMOS AL VECTOR LINKS
			(*this)+=link;
			links.push_back(link);



////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 1 = LINK[1]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[0] = ARTICULACION 1 ROTACIONAL
			SimpleJoint *auxJoint=new SimpleJoint(PI/2,-PI/2,true,0,Z_AXIS,false);
			auxJoint->setRelativePosition(Vector3D(0,0,0.52));
			auxJoint->LinkTo(links[0]);
			auxJoint->setValue(q_init[0]);
			joints.push_back(auxJoint);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 1");

//***********************************************************************//
// LA ARTICULACION 1 ESTA COMPUESTA POR UN CILINDRO Y DOS PRISMAS IRREG. //
//***********************************************************************//
			// CILINDRO(Cilindro)
			auxCyl=new CylindricalPart(0.06,0.425);//Altura y radio
			auxCyl->setColor(255,233,0);
			(*link)+=auxCyl;

			// PRISMA IRREGULAR DERECHO(Pris_Der)
			PrismaticPart *auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.155,0));
			list.push_back(Vector2D(0.155,0));
			list.push_back(Vector2D(0.155,0.5));
			list.push_back(Vector2D(0.125,0.55));
			list.push_back(Vector2D(-0.125,0.55));
			list.push_back(Vector2D(-155,0.5));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.1);
			auxPrism->setRelativePosition(Vector3D(-0.4,0,0.06));
			auxPrism->setRelativeOrientation(X_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,PI/2);
			(*link)+=auxPrism;

			// PRISMA IRREGULAR IZQUIERDO(Cil_Izq)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.155,0));
			list.push_back(Vector2D(0.155,0));
			list.push_back(Vector2D(0.155,0.5));
			list.push_back(Vector2D(0.125,0.55));
			list.push_back(Vector2D(-0.125,0.55));
			list.push_back(Vector2D(-155,0.5));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.1);
			auxPrism->setRelativePosition(Vector3D(0.3,0,0.06));
			auxPrism->setRelativeOrientation(X_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,PI/2);
			(*link)+=auxPrism;

			// SERVO2(Servo2)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.095,-0.2));
			list.push_back(Vector2D(0.095,-0.2));
			list.push_back(Vector2D(0.095,0.2));
			list.push_back(Vector2D(-0.095,0.2));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.35);
			(*link)+=auxPrism;

			// SERVO3(Servo3)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.095,-0.2));
			list.push_back(Vector2D(0.095,-0.2));
			list.push_back(Vector2D(0.095,0.2));
			list.push_back(Vector2D(-0.095,0.2));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.35);
			auxPrism->setRelativePosition(Vector3D(-0.29,0,0.3));
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;

			// AÑADIMOS AL VECTOR LINKS
			link->LinkTo(joints[0]);
			links.push_back(link);




////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 2 = LINK[2]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[1] = ARTICULACION 2 ROTACIONAL
			auxJoint=new SimpleJoint(25*PI/36 , -25*PI/36,true,0,Z_AXIS,false);
			auxJoint->setRelativePosition(Vector3D(0.29,0,1));
			auxJoint->setRelativeOrientation(0,-PI/2,0);
			auxJoint->LinkTo(joints[0]);
			auxJoint->setValue(q_init[1]);
			joints.push_back(auxJoint);
			//	joints[1]->setDrawReferenceSystem(true);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 2");

//***********************************************************************//
// LA ARTICULACION 2 ESTA COMPUESTA POR 4 CILINDROS Y 6 PRISMAS IRREG.   //
//***********************************************************************//

			// CILINDRO IZQUIERDO ABAJO(Cil_Ab_Izq)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0));
			(*link)+=auxCyl;

			// PRISMA IRREGULAR IZQUIERDA(Pris_Izq)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.13,0));
			list.push_back(Vector2D(0.13,0));
			list.push_back(Vector2D(0.13,1.1));
			list.push_back(Vector2D(-0.13,1.1));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.03);
			auxPrism->setRelativePosition(Vector3D(0,0,0));
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;

			// CILINDRO IZQUIERDO ARRIBA(Cil_Ar_Izq)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(1.1,0,0));
			(*link)+=auxCyl;


			// PRISMA IRREGULAR REFUERZO IZQUIERDA(Pris_Ref_Izq)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.4,0));
			list.push_back(Vector2D(0.4,0));
			list.push_back(Vector2D(0.315,0.04));
			list.push_back(Vector2D(-0.315,0.04));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.26);
			auxPrism->setRelativePosition(Vector3D(0.55,-0.13,0));
			auxPrism->setRelativeOrientation(X_AXIS,-PI/2);
			(*link)+=auxPrism;

			// PRISMA IRREGULAR CENTRO ABAJO(Pris_Cen_Ab)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(0,0));
			list.push_back(Vector2D(0.49,0));
			list.push_back(Vector2D(0.49,0.08));
			list.push_back(Vector2D(0.4,0.08));
			list.push_back(Vector2D(0.4,0.27));
			list.push_back(Vector2D(0.33,0.27));
			list.push_back(Vector2D(0.33,0.08));
			list.push_back(Vector2D(0.16,0.08));
			list.push_back(Vector2D(0.16,0.27));
			list.push_back(Vector2D(0.09,0.27));
			list.push_back(Vector2D(0.09,0.08));
			list.push_back(Vector2D(0,0.08));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.2);
			auxPrism->setRelativePosition(Vector3D(0.38,-0.1,0.03));
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;

			// PRISMA IRREGULAR CENTRO ARRIBA(Pris_Cen_Ar)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(0,0));
			list.push_back(Vector2D(0.49,0));
			list.push_back(Vector2D(0.49,0.08));
			list.push_back(Vector2D(0,0.08));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.2);
			auxPrism->setRelativePosition(Vector3D(0.64,-0.1,0.03));
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;

			// CILINDRO DERECHO ABAJO(Cil_Ab_Der)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0.52));
			(*link)+=auxCyl;

			// PRISMA IRREGULAR DERECHA(Pris_Der)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.13,0));
			list.push_back(Vector2D(0.13,0));
			list.push_back(Vector2D(0.13,1.1));
			list.push_back(Vector2D(-0.13,1.1));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.03);
			auxPrism->setRelativePosition(Vector3D(0,0,0.52));
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;


			// CILINDRO DERECHO ARRIBA(Cil_Ar_Der)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(1.1,0,0.52));
			(*link)+=auxCyl;

			// PRISMA IRREGULAR REFUERZO DERECHA(Pris_Ref_Der)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.4,0));
			list.push_back(Vector2D(0.4,0));
			list.push_back(Vector2D(0.315,0.04));
			list.push_back(Vector2D(-0.315,0.04));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.26);
			auxPrism->setRelativePosition(Vector3D(0.55,0.13,0.55));
			auxPrism->setRelativeOrientation(X_AXIS,PI/2);
			(*link)+=auxPrism;

			//CILINDRO SERVO PEQUEÑO(Servo_Peq)
			auxCyl=new CylindricalPart(0.07,0.10);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0.56));
			(*link)+=auxCyl;

			//CILINDRO SERVO GRANDE(Servo_Gran)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0.5));
			(*link)+=auxCyl;

			// AÑADIMOS AL VECTOR LINKS
			link->LinkTo(joints[1]);
			links.push_back(link);




////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 3 = LINK[3]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[2] = ARTICULACION 3 ROTACIONAL
			auxJoint=new SimpleJoint(25*PI/36 , -25*PI/36,true,0,Z_AXIS,false);
			auxJoint->setRelativePosition(Vector3D(0.18,0,2.1));//Desplazamiento a2
			auxJoint->setRelativeOrientation(0,-PI/2,0);
			auxJoint->LinkTo(joints[1]);
			auxJoint->setValue(q_init[2]);
			joints.push_back(auxJoint);
			//	joints[2]->setDrawReferenceSystem(true);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 3");

//******************************************************************* //
// LA ARTICULACION 3 ESTA COMPUESTA POR 1 CILINDRO Y 6 PRISMAS IRREG. //
//******************************************************************* //

			// PRISMA IRREGULAR CENTRO ARRIBA(Pris_Cen_Ar)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.16,0));
			list.push_back(Vector2D(0.16,0));
			list.push_back(Vector2D(0.16,0.065));
			list.push_back(Vector2D(-0.16,0.065));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.2);
			auxPrism->setRelativeOrientation(Y_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(0.56,0.1,0.22));
			(*link)+=auxPrism;

			//CILINDRO(Cilindro_Rojo)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0.38));
			(*link)+=auxCyl;

			// PRISMA IRREGULAR DERECHA(Pris_Der)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.125,0));
			list.push_back(Vector2D(0.125,0));
			list.push_back(Vector2D(0.125,0.97));
			list.push_back(Vector2D(-0.125,0.97));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.03);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(-0.01,0,0.38));
			(*link)+=auxPrism;

			// PRISMA IRREGULAR IZQUIERDA(Pris_Izq)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.155,0.18));
			list.push_back(Vector2D(0.155,-0.18));
			list.push_back(Vector2D(0.155,0.32));
			list.push_back(Vector2D(0.125,0.38));
			list.push_back(Vector2D(0.125,0.96));
			list.push_back(Vector2D(-0.125,0.96));
			list.push_back(Vector2D(-0.125,0.38));
			list.push_back(Vector2D(-0.155,0.32));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.06);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(0,0,0));
			(*link)+=auxPrism;

			// PRISMA IRREGULAR CENTRO ABAJO(Pris_Cen_Ab)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.16,0));
			list.push_back(Vector2D(0.16,0));
			list.push_back(Vector2D(0.16,0.065));
			list.push_back(Vector2D(0.1,0.065));
			list.push_back(Vector2D(0.1,0.18));
			list.push_back(Vector2D(0.045,0.18));
			list.push_back(Vector2D(0.045,0.065));
			list.push_back(Vector2D(-0.045,0.065));
			list.push_back(Vector2D(-0.045,0.18));
			list.push_back(Vector2D(-0.1,0.18));
			list.push_back(Vector2D(-0.1,0.065));
			list.push_back(Vector2D(-0.16,0.065));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.2);
			auxPrism->setRelativeOrientation(Y_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(0.38,0.1,0.22));
			(*link)+=auxPrism;

			// PRISMA IRREGULAR REFUERZO DERECHA(Pris_Ref_Der)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(0,0));
			list.push_back(Vector2D(0.04,0.045));
			list.push_back(Vector2D(0.04,0.81));
			list.push_back(Vector2D(0,0.81));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.2);
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(0.15,-0.13,0.41));
			(*link)+=auxPrism;

			// PRISMA IRREGULAR ARRIBA(Pris_Ar)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.16,0));
			list.push_back(Vector2D(-0.115,0));
			list.push_back(Vector2D(-0.115,0.05));
			list.push_back(Vector2D(0.115,0.05));
			list.push_back(Vector2D(0.155,0));
			list.push_back(Vector2D(0.16,0));
			list.push_back(Vector2D(0.16,0.11));
			list.push_back(Vector2D(-0.16,0.11));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativeOrientation(X_AXIS,-PI/2);
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(0.85,-0.13,0.22));
			(*link)+=auxPrism;

			//CILINDRO SERVO GRANDE(Servo_Gran)
			auxCyl=new CylindricalPart(0.03,0.175);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,-0.08));
			(*link)+=auxCyl;

			//CILINDRO SERVO PEQUEÑO(Servo_Peq)
			auxCyl=new CylindricalPart(0.06,0.175);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,-0.06));
			(*link)+=auxCyl;

			// SERVO4(Servo4)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.095,-0.2));
			list.push_back(Vector2D(0.095,-0.2));
			list.push_back(Vector2D(0.095,0.2));
			list.push_back(Vector2D(-0.095,0.2));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.35);
			auxPrism->setRelativePosition(Vector3D(0.07,0,-0.01));
			auxPrism->setRelativeOrientation(Z_AXIS,-PI/2);
			(*link)+=auxPrism;

			// SERVO5(Servo5)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.055,-0.11));
			list.push_back(Vector2D(0.055,-0.11));
			list.push_back(Vector2D(0.055,0.11));
			list.push_back(Vector2D(-0.055,0.11));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativePosition(Vector3D(0.67,0,0.23));
			auxPrism->setRelativeOrientation(Y_AXIS,PI/2);
			(*link)+=auxPrism;

			//CILINDRO REFUERZO SERVO(Cil_Ref)
			auxCyl=new CylindricalPart(0.06,0.08);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0.9,-0.15,0.22));
			auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
			(*link)+=auxCyl;

			// AÑADIMOS AL VECTOR LINKS
			link->LinkTo(joints[2]);
			links.push_back(link);




////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 4 = LINK[4]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[3] = ARTICULACION 4 ROTACIONAL
			auxJoint=new SimpleJoint(7*PI/9 , -7*PI/9,true,0,Z_AXIS,0);
			auxJoint->setRelativePosition(Vector3D(-0.05,0.03,3.1));
			auxJoint->LinkTo(joints[2]);
			auxJoint->setValue(q_init[3]);
			joints.push_back(auxJoint);
			//	joints[3]->setDrawReferenceSystem(true);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 4");

			//******************************************************************** //
			// LA ARTICULACION 4 ESTA COMPUESTA POR 2 CILINDROS Y 2 PRISMAS IRREG. //
			//******************************************************************** //

			// PRISMA IRREGULAR(Prisma)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.125,0));
			list.push_back(Vector2D(0.2,0));
			list.push_back(Vector2D(0.26,0.03));
			list.push_back(Vector2D(0.29,0.09));
			list.push_back(Vector2D(0.29,0.23));
			list.push_back(Vector2D(0.26,0.275));
			list.push_back(Vector2D(0.23,0.275));
			list.push_back(Vector2D(0.23,0.11));
			list.push_back(Vector2D(0.2,0.11));
			list.push_back(Vector2D(0.2,0.06));
			list.push_back(Vector2D(0.09,0.06));
			list.push_back(Vector2D(0.09,0.03));
			list.push_back(Vector2D(-0.09,0.03));
			list.push_back(Vector2D(-0.09,0.06));
			list.push_back(Vector2D(-0.125,0.06));
			list.push_back(Vector2D(-0.125,0.11));
			list.push_back(Vector2D(-0.155,0.11));
			list.push_back(Vector2D(-0.155,0.13));
			list.push_back(Vector2D(-0.185,0.13));
			list.push_back(Vector2D(-0.21,0.09));
			list.push_back(Vector2D(-0.185,0.03));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativeOrientation(X_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,-PI);
			auxPrism->setRelativePosition(Vector3D(0,-0.1,0));
			(*link)+=auxPrism;

			// PRISMA IRREGULAR IZQUIERDA(Pris_Izq)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.045,0));
			list.push_back(Vector2D(0.045,0));
			list.push_back(Vector2D(0.045,0.125));
			list.push_back(Vector2D(-0.045,0.125));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,233,0);
			auxPrism->setHeight(0.03);
			auxPrism->setRelativeOrientation(Y_AXIS,PI/2);
			auxPrism->setRelativePosition(Vector3D(0.16,-0.04,0.16));
			(*link)+=auxPrism;

			//CILINDRO IZQUIERDA(Cil_Izq)
			auxCyl=new CylindricalPart(0.03,0.065);
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(0.16,0.2,0.2));
			auxCyl->setRelativeOrientation(0,PI/2,0);
			(*link)+=auxCyl;

			//CILINDRO DERECHA(Cil_Der)
			auxCyl=new CylindricalPart(0.03,0.175);
			auxCyl->setColor(255,233,0);
			auxCyl->setRelativePosition(Vector3D(-0.26,0.02,0.27));
			auxCyl->setRelativeOrientation(0,PI/2,0);
			(*link)+=auxCyl;


			//CILINDRO SERVO ARRIBA(Servo_Ar)
			auxCyl=new CylindricalPart(0.09,0.05);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(-0.25,0.02,0.27));
			auxCyl->setRelativeOrientation(Y_AXIS,PI/2);
			(*link)+=auxCyl;

			//CILINDRO SERVO ABAJO(Servo_Ab)
			auxCyl=new CylindricalPart(0.05,0.05);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,-0.04));
			(*link)+=auxCyl;

			// SERVO6(Servo6)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.055,-0.11));
			list.push_back(Vector2D(0.055,-0.11));
			list.push_back(Vector2D(0.055,0.11));
			list.push_back(Vector2D(-0.055,0.11));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativePosition(Vector3D(-0.24,0.03,0.22));
			auxPrism->setRelativeOrientation(X_AXIS,-PI/2);
			auxPrism->setRelativeOrientation(Z_AXIS,PI/2);
			(*link)+=auxPrism;

			// AÑADIMOS AL VECTOR LINKS
			link->LinkTo(joints[3]);
			links.push_back(link);





////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 5 = LINK[5]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[4] = ARTICULACION 5 ROTACIONAL
			auxJoint=new SimpleJoint(5*PI/9,-5*PI/9,true,0,Z_AXIS,false);
			auxJoint->setRelativePosition(Vector3D(-0.22,0,3.4));
			auxJoint->setRelativeOrientation(PI,-PI/2,0);
			auxJoint->LinkTo(joints[3]);
			auxJoint->setValue(q_init[4]);
			joints.push_back(auxJoint);
			//	joints[4]->setDrawReferenceSystem(true);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 5");

			//******************************************************************* //
			// LA ARTICULACION 5 ESTA COMPUESTA POR 3 CILINDROS Y 1 PRISMA IRREG. //
			//******************************************************************* //

			// PRISMA IRREGULAR(Prisma)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.17,0));
			list.push_back(Vector2D(0.17,0));
			list.push_back(Vector2D(0.21,0.02));
			list.push_back(Vector2D(0.23,0.06));
			list.push_back(Vector2D(0.23,0.1));
			list.push_back(Vector2D(0.19,0.19));
			list.push_back(Vector2D(0.17,0.19));
			list.push_back(Vector2D(0.17,0.1));
			list.push_back(Vector2D(0.14,0.1));
			list.push_back(Vector2D(0.14,0.06));
			list.push_back(Vector2D(-0.14,0.06));
			list.push_back(Vector2D(-0.14,0.1));
			list.push_back(Vector2D(-0.17,0.1));
			list.push_back(Vector2D(-0.07,0.19));
			list.push_back(Vector2D(-0.19,0.19));
			list.push_back(Vector2D(-0.23,0.1));
			list.push_back(Vector2D(-0.23,0.06));
			list.push_back(Vector2D(-0.21,0.02));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(255,255,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativeOrientation(X_AXIS,PI/2);
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			auxPrism->setRelativePosition(Vector3D(-0.19,-0.12,0.19));
			(*link)+=auxPrism;

			// CILINDRO DERECHA(Cil_Der)
			auxCyl=new CylindricalPart(0.02,0.12);
			auxCyl->setColor(233,233,0);
			(*link)+=auxCyl;

			//CILINDRO IZQUIERDA(Cil_Izq)
			auxCyl=new CylindricalPart(0.02,0.12);
			auxCyl->setColor(233,233,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0.36));
			(*link)+=auxCyl;


			//CILINDRO REFUERZO(Cil_Ref)
			auxCyl=new CylindricalPart(0.06,0.08);
			auxCyl->setColor(233,233,0);
			auxCyl->setRelativePosition(Vector3D(-0.13,0.15,0.20));
			auxCyl->setRelativeOrientation(Y_AXIS,-PI/2);
			(*link)+=auxCyl;

			// SERVO7(Servo7)
			auxPrism=new PrismaticPart;
			list.push_back(Vector2D(-0.055,-0.11));
			list.push_back(Vector2D(0.055,-0.11));
			list.push_back(Vector2D(0.055,0.11));
			list.push_back(Vector2D(-0.055,0.11));
			auxPrism->setPolygonalBase(list);
			list.clear();
			auxPrism->setColor(0,0,0);
			auxPrism->setHeight(0.25);
			auxPrism->setRelativePosition(Vector3D(-0.15,0.05,0.2));
			auxPrism->setRelativeOrientation(Y_AXIS,-PI/2);
			(*link)+=auxPrism;

			// AÑADIMOS AL VECTOR LINKS
			link->LinkTo(joints[4]);
			links.push_back(link);





////////////////////////////////////////////////////////////////////////////
//////////////////////////ARTICULACION 6 = LINK[6]//////////////////////////
////////////////////////////////////////////////////////////////////////////

			//JOINT[5] = ARTICULACION 6 ROTACIONAL
			auxJoint=new SimpleJoint(-PI,PI,true,0,Z_AXIS,false);
			auxJoint->setRelativePosition(Vector3D(-0.04,0,3.59));
			auxJoint->LinkTo(joints[4]);
			auxJoint->setValue(q_init[5]);
			joints.push_back(auxJoint);
			//	joints[5]->setDrawReferenceSystem(true);

			//NUEVO GRUPO DE PIEZAS
			link=new ComposedEntity;
			link->setName("Link 6");

			//************************************************* //
			// LA ARTICULACION 6 ESTA COMPUESTA POR UN CILINDRO //
			//************************************************* //
			// CILINDRO:
			auxCyl=new CylindricalPart(0.06,0.03);//Altura y radio
			auxCyl->setColor(0,0,0);
			auxCyl->setRelativePosition(Vector3D(0,0,0));
			(*link)+=auxCyl;

			// AÑADIMOS AL VECTOR LINKS
			auxCyl->LinkTo(joints[5]);
			links.push_back(auxCyl);



			//Tcp
			tcp=new Tcp();
			tcp->setName("Tcp");
			tcp->setRelativePosition(Vector3D(0,0,0.06));
			tcp->LinkTo(joints[5]);
			tcp->setDrawReferenceSystem(true);

			getConfigurationOf(q_init,conf);

			//joints[0]->setSimulationParameters(PI/12);//	15º/seg
			//joints[1]->setSimulationParameters(23*PI/36);//	115º/seg
			//joints[2]->setSimulationParameters(23*PI/36);//	115º/seg
			//joints[3]->setSimulationParameters(14*PI/9);//	280º/seg
			//joints[4]->setSimulationParameters(5*PI/3);//	300º/seg
			//joints[5]->setSimulationParameters(5*PI/3);//	300º/seg


			(*this)+=links[0];

		}

		EuitiBotSim::~EuitiBotSim(void)
		{
		}

		bool EuitiBotSim::getCoordinatesOf(vector<double> &_q)
		{//TODO
		return true;
		}



		bool EuitiBotSim::inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf)
		{//TODO
		return true;
		}



		bool EuitiBotSim::getConfigurationOf(const vector<double> &_q, unsigned char &_conf)
		{//TODO
		return true;
		}



		bool EuitiBotSim::Pruebas()
		{
		//TODO
		return true;
		}



		bool EuitiBotSim::configuration(unsigned char _conf, double& _s, double& _e, double& _w)
		{
		//TODO
		return true;
		}



		bool EuitiBotSim::configuration(double _s, double _e, double _w, unsigned char &_conf)
		{
		//TODO
		return true;
		}






		void EuitiBotSim::simulate(double delta_t)
		{
		//TODO

		}


		void EuitiBotSim::goTo(vector<double> q)
		{
		//TODO

		}



};//Namespace mr
