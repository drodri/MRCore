#include "wheeledbasesim.h"
#include <iostream>
#include "gl/gltools.h"

#include "base/logger.h"
#include "../world/world.h"
#include "../world/cylindricalpart.h"
namespace mr
{
IMPLEMENT_MR_OBJECT(WheeledBaseSim)
WheeledBaseSim::WheeledBaseSim(double w, double l, double wh_radius, double wh_width){
	width=w;
	large=l;
	wheel_radius=wh_radius;//total outer radius
	wheel_width=wh_width;
	//wheels centers
	wheels[0]=Vector3D(large/2,(width+wheel_width)/2,wheel_radius);
	wheels[1]=Vector3D(large/2,-(width+wheel_width)/2,wheel_radius);
	wheels[2]=Vector3D(-large/2,(width+wheel_width)/2,wheel_radius);
	wheels[3]=Vector3D(-large/2,-(width+wheel_width)/2,wheel_radius);
}
//serializers
void WheeledBaseSim::writeToStream(Stream& stream)
{
	//not saving as composed entity because it is not needed.. object are created in the constructor
	//if there are specific implementations
	SolidEntity::writeToStream(stream);
	stream<<width<<large<<wheel_radius<<wheel_width;
	stream<<speed<<rotSpeed;
	odometry.writeToStream(stream);
	
}
void WheeledBaseSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
	stream>>width>>large>>wheel_radius>>wheel_width;
	stream>>speed>>rotSpeed;
	odometry.readFromStream(stream);
	//specific initializations
	wheels[0]=Vector3D(large/2,(width+wheel_width)/2,wheel_radius);
	wheels[1]=Vector3D(large/2,-(width+wheel_width)/2,wheel_radius);
	wheels[2]=Vector3D(-large/2,(width+wheel_width)/2,wheel_radius);
	wheels[3]=Vector3D(-large/2,-(width+wheel_width)/2,wheel_radius);
}
void WheeledBaseSim::simulate(double delta_t)
{
	
	//aqui es necesario recoger el mundo al que pertenece el world
	double delta_x=speed*delta_t;
	double delta_y=0;
	double delta_th=rotSpeed*delta_t;
	
	/*verificar si la posicion de destino es valida: se verifica
	de manera simple lanzando unos rayos  abajo segun el eje Z desde los centros de las ruedas
	para ello hay que discretizar la trayectoria en trozitos del tamaño de un radio como maximo
	normalemnete el tiempo de simulación es mucho menor
	Se obtiene entonces cuatro alturas. De estas se utilizan las tres más estables segun un modelo dinámico
	sencillo. Además se verifica que el salto dado por las ruedas de apoyo no supera su diametro .
	en cuyo caso se actualiza la posición y la pose. La pose se actualiza con el movimiento lineal,
	mientras que la posición puede ser 3d (subir una rampa por ejemplo
	*/
	Transformation3D position=getAbsoluteT3D();
	Transformation3D delta(delta_x*cos(delta_th),delta_x*sin(delta_th),0,0,0,delta_th);
	//the new posible absolute position is:
	Transformation3D newposition=position*delta;


	//el calculo de posicion de destino debera ir en una funcion para poder ser reusado
	//base del planificador RRT:
	if(computeGroundedLocation(newposition)==false)return;
	//actualizamos la posición
	setAbsoluteT3D(newposition);
	//verifico que no hay colisión (aunque habría que desactivar la deteccion con las ruedas)
	World *world=getWorld();
	if(world){
		if(world->checkCollisionWith(*this)){
			setAbsoluteT3D(position); //no muevo el robot
			return ;
		}
	}
	//esta es la posicion teórica simple de los encoders en caso de que el robot pueda moverse
	odometry.pose*=delta;
	odometry.timeStamp();
}
void WheeledBaseSim::drawGL()
{
//constructor: create a physical representation of the robot only for drawing
	double 	height=wheel_radius*3;
	double 	clearance=wheel_radius;
vector<Vector2D> list;
PrismaticPart bod;
list.push_back(Vector2D(large/2,width/2));
list.push_back(Vector2D(large/2,-width/2));
list.push_back(Vector2D(-large/2,-width/2));
list.push_back(Vector2D(-large/2,width/2));
bod.setPolygonalBase(list);
bod.setHeight(height-clearance);
bod.setRelativePosition(Vector3D(0,0,clearance));
bod.setColor(1,0,0);

//las ruedas no se pueden añadir hasta no tener un mecanismo de exclusión de detección
CylindricalPart wheel(wheel_width,wheel_radius);
wheel.setColor(0.1,0.1,0.1);


glPushMatrix();	
	getAbsoluteT3D().transformGL();
	//body
	bod.drawGL();
	//each wheel
	wheel.setRelativeOrientation(X_AXIS,-PI/2);
	wheel.setRelativePosition(Vector3D(large/2,width/2,wheel_radius+0.002));
	wheel.drawGL();

	wheel.setRelativeOrientation(X_AXIS,-PI/2);
	wheel.setRelativePosition(Vector3D(-large/2,width/2,wheel_radius+0.002));
	wheel.drawGL();

	wheel.setRelativeOrientation(X_AXIS,PI/2);
	wheel.setRelativePosition(Vector3D(large/2,-width/2,wheel_radius+0.002));
	wheel.drawGL();

	wheel.setRelativeOrientation(X_AXIS,PI/2);
	wheel.setRelativePosition(Vector3D(-large/2,-width/2,wheel_radius+0.002));
	wheel.drawGL();
glPopMatrix();	
	
	return;
}
bool WheeledBaseSim::getOdometry(Odometry& p)
{
	p=odometry;
	return true;
}
bool WheeledBaseSim::move(double s, double rot)
{
	speed=s;
	rotSpeed=rot;
	if(speed>2.0)speed=2.0;
	if(speed<-2.0)speed=-2.0;
	if(rotSpeed>1.0)rotSpeed=1.0;
	if(rotSpeed<-1.0)rotSpeed=-1.0;
	
	return true;
}

bool WheeledBaseSim::computeGroundedLocation(Transformation3D &p,World* w)
{
//partiendo de p, la modifica para que sea una posición geométricamente válida, suponiendo una
//gravedad, pero sin consideraciones dinámicas

	int i;
	
	//absolute center of the four wheels
	Vector3D abswheels[4];
	double dw[4]={10.0,10.0,10.0,10.0};
	bool bw[4];
	//retrieving the needed data
	Transformation3D t=getAbsoluteT3D(); //current location
	for(i=0;i<4;i++)abswheels[i]=p*wheels[i]; 
	Vector3D uz=(p.getVectorW())*(-1.0);
	
	if(w==0)w=getWorld();
	World* world=w;
	

	if(world){
		//desactivo el calculo de intersecciones con mi modelo, para que no vea mis propias piezas
		setIntersectable(false);
		for(i=0;i<4;i++)bw[i]=world->rayIntersection(abswheels[i],uz,dw[i]);
		setIntersectable();
		//ya tengo calculados los puntos de contacto. descarto los que superan una rueda
		for(i=0;i<4;i++)
			if(dw[i]>2*wheel_radius)
				bw[i]=false;

		//si son menos de 3... retorna como posicion invalida
		int nc=(bw[0]*1+bw[1]*1+bw[2]*1+bw[3]*1);
		if(nc<3)return false;

		//selecciono los dos menores: ordeno por indices
		int ord[4]={0,1,2,3};
		for(i=1;i<4;i++)if(dw[ord[i]]<dw[ord[0]]){int c=ord[0];ord[0]=ord[i];ord[i]=c;}
		for(i=2;i<4;i++)if(dw[ord[i]]<dw[ord[1]]){int c=ord[1];ord[1]=ord[i];ord[i]=c;}
		if(dw[ord[3]]<dw[ord[2]]){int c=ord[2];ord[2]=ord[3];ord[3]=c;}
		//calculo los punto de contacto teóricos cayendo en z:
		Vector3D contact[4];
		for(i=0;i<4;i++)contact[i]=abswheels[i]+uz*dw[i];
		//obtengo el vector del balancín con los dos más altos
		if(nc==4){
			Vector3D axis=(contact[ord[0]]-contact[ord[1]]).getUnitaryVector();
			Vector3D n=t.getVectorW();
			Vector3D nn=n-axis*(axis*n);
			//de los dos restantes proyecto sobre la nn y me quedo con el menor valor 
			double d2=nn*(contact[ord[0]]-contact[ord[2]]);
			double d3=nn*(contact[ord[0]]-contact[ord[3]]);
			if(d3<d2)ord[2]=ord[3];
		}
		//ahora los tres puntos estan obtenidos: lo transformamos en un sdr coherente con la plataforma
		bool flag[4]={false,false,false,false};
		for(i=0;i<3;i++)flag[ord[i]]=true;
		Vector3D vu,vv,vw,npos;
		//nueva posición:
		if(flag[0]&&flag[3])npos=(contact[0]+contact[3])*0.5;
		if(flag[1]&&flag[2])npos=(contact[1]+contact[2])*0.5;
		//nueva orientación:
		if(flag[0]&&flag[2])vu=(contact[0]-contact[2]).getUnitaryVector();
		if(flag[1]&&flag[3])vu=(contact[1]-contact[3]).getUnitaryVector();
		if(flag[0]&&flag[1])vv=(contact[0]-contact[1]).getUnitaryVector();
		if(flag[2]&&flag[3])vv=(contact[2]-contact[3]).getUnitaryVector();
		p.position=npos;
		p.orientation=OrientationMatrix(vu,vv);
	}
return true;
}

	//Methods for validating possible wheeled base positions
bool WheeledBaseSim::dropWheeledBase(Transformation3D &t, World *w)
	{
		
		if(!w)w=this->getWorld();

		Vector3D wheels[4];

		double dw[4]={10000.0,10000.0,10000.0,10000.0};
		bool bw[4];
		//t.position.z+=robot->getWheelRadius();
		setRelativeT3D(t);
		getWheelsCenterPoints(wheels);
		setIntersectable(false);
		Vector3D uz(0,0,-1);
		for(int j=0;j<4;j++)bw[j]=w->rayIntersection(wheels[j],uz,dw[j]);

		//si son menos de 3 lo considero como posicion invalida
		int nc=(bw[0]*1+bw[1]*1+bw[2]*1+bw[3]*1);
		if(nc<3)return false;

		//selecciono los dos menores: ordeno por indices
		int ord[4]={0,1,2,3};
		int i;
		for(i=1;i<4;i++)if(dw[ord[i]]<dw[ord[0]]){int c=ord[0];ord[0]=ord[i];ord[i]=c;}
		for(i=2;i<4;i++)if(dw[ord[i]]<dw[ord[1]]){int c=ord[1];ord[1]=ord[i];ord[i]=c;}
		if(dw[ord[3]]<dw[ord[2]]){int c=ord[2];ord[2]=ord[3];ord[3]=c;}
		//calculo los punto de contacto teóricos cayendo en z:
		Vector3D contact[4];
		for(i=0;i<4;i++)contact[i]=wheels[i]+uz*dw[i];
		//obtengo el vector del balancín con los dos más altos
		if(nc==4){
			Vector3D axis=(contact[ord[0]]-contact[ord[1]]).getUnitaryVector();
			Vector3D n=t.getVectorW();
			Vector3D nn=n-axis*(axis*n);
			//de los dos restantes proyecto sobre la nn y me quedo con el menor valor 
			double d2=nn*(contact[ord[0]]-contact[ord[2]]);
			double d3=nn*(contact[ord[0]]-contact[ord[3]]);
			if(d3<d2)ord[2]=ord[3];
		}
		//ahora los tres puntos estan obtenidos: lo transformamos en un sdr coherente con la plataforma
		bool flag[4]={false,false,false,false};
		for(i=0;i<3;i++)flag[ord[i]]=true;
		Vector3D vu,vv,vw,npos;
		//nueva posición:
		if(flag[0]&&flag[3])npos=(contact[0]+contact[3])*0.5;
		if(flag[1]&&flag[2])npos=(contact[1]+contact[2])*0.5;
		//nueva orientación:
		if(flag[0]&&flag[2])vu=(contact[0]-contact[2]).getUnitaryVector();
		if(flag[1]&&flag[3])vu=(contact[1]-contact[3]).getUnitaryVector();
		if(flag[0]&&flag[1])vv=(contact[0]-contact[1]).getUnitaryVector();
		if(flag[2]&&flag[3])vv=(contact[2]-contact[3]).getUnitaryVector();

		Transformation3D drop;
		drop.position=npos;
		drop.orientation=OrientationMatrix(vu,vv);

		if(Vector3D(0,0,1)*drop.getVectorW()<0.7)return false; //maximum admisible inclination (45º)
		t=drop;
		setRelativeT3D(t);
		return true;
	}

}; //Namespace mr