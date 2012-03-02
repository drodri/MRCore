#include "personsim.h"
#include <iostream>
#include "gl/gltools.h"
#include "../world/cylindricalpart.h"
#include "../world/spherepart.h"
#include "world/world.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(PersonSim)

PersonSim::PersonSim(float h)
{
	height=h;
	speed=rotSpeed=1.5;
	radius=height/9.0f;//0.20;
	float lenBody=(height-2*radius)/2.0f;
	float rLeg=radius/2.5;
	//las ruedas no se pueden añadir hasta no tener un mecanismo de exclusión de detección
	CylindricalPart *wheel1=new CylindricalPart(lenBody, radius);
	wheel1->setColor(1.0,0.3,0.3);
	wheel1->setRelativeOrientation(Z_AXIS,-PI/2);
	wheel1->setRelativePosition(Vector3D(0,0,lenBody));

	CylindricalPart *leg1=new CylindricalPart(lenBody, rLeg);
	leg1->setColor(1.0,0.3,0.3);
	leg1->setRelativeOrientation(Z_AXIS,-PI/2);
	leg1->setRelativePosition(Vector3D(0,rLeg,0));

	CylindricalPart *leg2=new CylindricalPart(lenBody, rLeg);
	leg2->setColor(1.0,0.3,0.3);
	leg2->setRelativeOrientation(Z_AXIS,-PI/2);
	leg2->setRelativePosition(Vector3D(0,-rLeg,0));

	SpherePart *head=new SpherePart(radius);
	head->setRelativePosition(Vector3D(0,0,height-radius));
	head->setColor(1.0,0.3,0.3);

	(*this)+=wheel1;
	(*this)+=head;
	(*this)+=leg1;
	(*this)+=leg2;
}
void PersonSim::writeToStream(Stream& stream)
{
	
}
void PersonSim::readFromStream(Stream& stream)
{
	
}
void PersonSim::drawGL()
{
	//it si possible to make the drawing independent of the geometric model
	//so we overwrite the composedentity drawing function with this one
	ComposedEntity::drawGL();
	return;
}
bool PersonSim::computeGroundedLocation(Transformation3D &p)
{
//partiendo de p, la modifica para que sea una posición geométricamente válida, suponiendo una
//gravedad, pero sin consideraciones dinámicas
	cout<<"compute ground:  "<<p<<endl;
	int i;
	
	//absolute center of the four wheels
	Vector3D abswheels;
	double dw=10.0;
	bool bw;
	//retrieving the needed data
//	Transformation3D t=getAbsoluteT3D(); //current location
	abswheels=p.position; 
	Vector3D uz=(p.getVectorW())*(-1.0);
	World *world=getWorld();

	if(world){
		//desactivo el calculo de intersecciones con mi modelo, para que no vea mis propias piezas
		setIntersectable(false);
		bw=world->rayIntersection(abswheels,uz,dw);
		setIntersectable();
		//ya tengo calculados los puntos de contacto. descarto los que superan una rueda
		if(dw>2*radius)bw=false;


		//calculo los punto de contacto teóricos cayendo en z:
		Vector3D contact=abswheels+uz*dw;
		
		//ahora los tres puntos estan obtenidos: lo transformamos en un sdr coherente con la plataforma
	//	bool flag[4]={false,false,false,false};
	//	for(i=0;i<3;i++)flag[ord[i]]=true;
		Vector3D vu,vv,vw,npos;
		//nueva posición:
		cout<<"The new pos is: "<<contact<<endl;
		npos=contact;
		p.position=npos;
		//p.orientation=OrientationMatrix(vu,vv);
	}
	return true;
}
void PersonSim::simulate(double delta_t)
{
//	cout<<"simulating: "<<speed<<" "<<rotSpeed<<" "<<delta_t<<endl;
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
//	if(computeGroundedLocation(newposition)==false)return;
	//actualizamos la posición
	setAbsoluteT3D(newposition);
	//verifico que no hay colisión (aunque habría que desactivar la deteccion con las ruedas)
	World *world=getWorld();
	if(world){
		if(world->checkCollisionWith(*this)){
			setAbsoluteT3D(position); //no muevo el robot
			rotSpeed=rand()/(float)RAND_MAX;
			speed=-speed;
			return ;
		}
	}
	//esta es la posicion teórica simple de los encoders en caso de que el robot pueda moverse
//	pose=pose*Pose(delta_x,delta_y,delta_th);

	int r=rand()%75;
	if(r==0)
	{
		rotSpeed=rand()/(float)RAND_MAX;
		speed=rand()/(float)RAND_MAX;
	}
	else if(r==1)
		rotSpeed=0;
	else if(r==2)
		speed=0;

}


}; //Namespace mr