
//WB_STATE DEFINITIONS


#include "wbstate.h"



bool WBState::isEqual(RobotState *n)
{
//iguales si son compatibles:
//primera condición: poses a una distancia menor que radio de rueda
	WBState *naux=dynamic_cast<WBState *>(n);
	if(!naux)return false;
	if(robot==0)return false;
	Vector3D dif=naux->pose-pose;
	if(dif*dif>0.2*robot->getWheelRadius()*robot->getWheelRadius())return false;

	//TODO hay que ver que los angulos son compatibles tb
	//de momento retorno true
	return true;
}
//check if it is a valid state
bool WBState::isValid()
{
//as it is imposible to have a valid RobotState
//without sectors... only if it has sectors is checked
	if(sectors.empty())return false;
return true;
}

//metric used to evaluate de distances: it could be any measure, but
//the less the better
double WBState::distanceTo(RobotState *p)
{
	WBState *naux=dynamic_cast<WBState *>(p);
	if(!naux)return 10000.0;
	Vector3D aux=naux->pose-pose;
	double val=aux*aux;
	if(2*aux.z*aux.z>val)val=2*aux.z*aux.z;
	//val*=(2*PI/(2*PI+naux->getSize()+getSize()));

	double h=2*robot->getWheelRadius();
	Segment3D segm=Segment3D(pose+Vector3D(0,0,h),naux->pose+Vector3D(0,0,h));
	robot->setIntersectable(false);
	//penalties the intersection
	if(world->segmentIntersection(segm,0))val*=8;
	robot->setIntersectable(true);
	return val;
}

//returns true if the robot state is equal to the state
bool WBState::isEqualToCurrentRobotState()
{
//Transformation3D t=robot->getAbsoluteT3D(); 
//return
Transformation3D t=robot->getAbsoluteT3D();
	Vector3D dif=pose-t.position;
	if(dif*dif>robot->getWheelRadius()*robot->getWheelRadius())return false;
return true;
}
//create a new state from current robot position
RobotState *WBState::createStateFromCurrentRobotState()
{
if(!world)return 0;
if(!robot)return 0;
vector<double> v(4);
Transformation3D t=robot->getAbsoluteT3D();
double r,p,y;

v[0]=t.position.x;
v[1]=t.position.y;
v[2]=t.position.z;
t.orientation.getRPY(r,p,y);
v[3]=y;
WBState *aux= (WBState *)createStateFromSample(v);
if(aux)aux->specialize(t);
return aux;
}
WBState *WBState::createStateFromPoint3D(double x,double y,double z)
{
vector<double> aux(3);
aux[0]=x;aux[1]=y;aux[2]=z;
return (WBState *)createStateFromSample(aux);
}
//creates a robot state with a set of doubles. if invalid return false
RobotState *WBState::createStateFromSample(vector<double> values)
{
	//valid conditions: there is a robt and a world defined, there are 3 or 4 doubles
	if(!world)return 0;
	if(!robot)return 0;
	if(values.size()<3)return 0;
//two options: a 3D position, and a 3D position + orientation
	double val(-11*PI/12),inc(PI/6),half_inc(PI/12.5);
	bool nuevoSector=false;
	Transformation3D t;
	int i=0;
	bool flag=true;
	double hmax;
	WheeledBaseSim *r=robot;
	/*primero selecciono la altura más alta valida... que sera a partir de quien genero*/
	//Si tengo 3 argumentos... la altura queda determinada por el más alto de los validos
	//si tengo más de 3 (4), la altura queda determinada por la orientación indicada (si es valida, claro)
	if(values.size()==3)
	{
		//bucle para detectar el hmax
		for(i=0;i<12;i++)
		{
			t=Transformation3D(values[0],values[1],values[2],Z_AXIS,i*inc+half_inc);
			if(checkPose(t)){ ///revisar ¡que pasa con t!
				if(flag)hmax=t.position.z;
				flag=false;
				if(t.position.z>hmax)hmax=t.position.z;
			}
		}
		if(flag)return 0; //no valid poses for that coordinates. 
	}
	else
	{ //hmax is the z, after the drop, if there is no valid pose.. returns 0
		t=Transformation3D(values[0],values[1],values[2],Z_AXIS,values[3]);
		if(checkPose(t))hmax=t.position.z;
		else return 0;

	}
	WBState aux(r,world);
	//Create the valid sectors
	hmax+=2*r->getWheelRadius();
	for(i=0;i<12;i++)
	{
		t=Transformation3D(values[0],values[1],hmax,Z_AXIS,i*inc+half_inc);
		if(checkPose(t)){ ///revisar ¡que pasa con t!
			//creo un sector
			Sector newSector;
			newSector.min=i*inc;
			newSector.max=(i+1)*inc;
			newSector.pose=t;
			if(t.position.z>hmax)hmax=t.position.z;
			aux.sectors.push_back(newSector);
		}
	}
	

	//utilizo el pos mas alto y elimino todos los sectores que esten por debajo una distancia mayor que
	//la admitida por la gota que es el radio de la rueda del robot
	
	for(i=(int)aux.sectors.size()-1;i>=0;i--)
		if(hmax-aux.sectors[i].pose.position.z>(4*r->getWheelRadius()))aux.sectors.erase(aux.sectors.begin()+i);

	if(aux.sectors.empty())return 0;
	
	//At his point we obtain the medium position value(X, Y are the same but, 
	Vector3D newPose(0,0,0);
	int numvs=aux.sectors.size();
	double factorns=1.0/((double)numvs);
	for(i=0;i<numvs;i++)newPose=newPose+((aux.sectors[i].pose.position)*factorns);

	
	//fusión de sectores
	i=1;
	while(i<(int)aux.sectors.size())
	{  
		if(aux.sectors[i].min-aux.sectors[i-1].max<EPS){
			aux.sectors[i-1].max=aux.sectors[i].max;
			aux.sectors.erase(aux.sectors.begin()+i);
		}
		else i++;
	}
	//creo el nuevo estado
	aux.pose=newPose;
	return new WBState(aux);
	
}
vector<double> WBState::getSample()
{
	vector<double> v(4);
	v[0]=pose.x;
	v[1]=pose.y;
	v[2]=pose.z;
	if(sectors.empty()==false)
	v[3]=(sectors[0].min+sectors[0].max)*0.5;
	return v;
}

bool WBState::checkPose(Transformation3D &t)
{
		Vector3D wheels[4];

		double dw[4]={10000.0,10000.0,10000.0,10000.0};
		bool bw[4];
		//t.position.z+=robot->getWheelRadius();
		robot->setRelativeT3D(t);
		robot->getWheelsCenterPoints(wheels);
		robot->setIntersectable(false);
		Vector3D uz(0,0,-1);
		for(int j=0;j<4;j++)bw[j]=world->rayIntersection(wheels[j],uz,dw[j]);

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
		robot->setRelativeT3D(t);
		return true;
}




//moves the robot one step from the current position towards
//the state. Local planner
bool WBState::moveRobotCloser(double stepSize)
{
Transformation3D t=robot->getAbsoluteT3D();
Vector3D ux=t.orientation.getVectorU();
Vector3D dif=pose-t.position;
//si el vector dif tiene respecto del plano horizontal mas de 45º... considero inabordable la escalada
if(dif.x*dif.x+dif.y*dif.y<2*dif.z*dif.z)return false; //end of move
//if the robot is in this node... doesn´t move

if(isEqualToCurrentRobotState())return false;

//linear speed
double distance=dif.module();
double speed=1;
double timeinterval=stepSize/speed;
if(stepSize>distance)speed=distance/timeinterval;

//angular speed
Vector3D udif=dif.getUnitaryVector();
double angle1=Angle::makePositive(atan2(ux.y,ux.x));
double angle2=Angle::makePositive(atan2(udif.y,udif.x));
Angle angle((angle2-angle1));
//velocidad de giro max 1 rad/s
double rotSpeed=angle.getValue();
if(rotSpeed>1)rotSpeed=1;
if(rotSpeed<-1)rotSpeed=-1;
if(rotSpeed*timeinterval>angle.getValue())rotSpeed=angle.getValue()/timeinterval;
if((fabs(rotSpeed)<EPS)&&(speed<EPS))return false;
//correccion para evitar el movimiento circular:
if(speed>EPS){
	double factor=fabs(rotSpeed/speed);
	if((factor<3*PI)&&(factor>PI))speed/=3;
}
////////////////////move the robot///////////////////
robot->move(speed,rotSpeed);
robot->simulate(timeinterval);

//verifico que se ha movido
Transformation3D tn=robot->getAbsoluteT3D();
//revisar la condición de que se ha movido

if(((tn.position-t.position).module()<EPS)&&
   (tn.orientation.getVectorU()*t.orientation.getVectorV()<EPS))return false;

//dif=target->pose-t.position;
//condicion muy dura: debe acercarse obligatoriamente si no... deja de planificar
//if(dif.module()>distance)return false;
return true;
}
//place the robot in a state
void WBState::placeRobot()
{
	double angle=0;
	if(sectors.size()>0){
		angle=0.5*(sectors[0].min+sectors[0].max);
	}
	Transformation3D t(pose.x,pose.y,pose.z,Z_AXIS,angle);
	//robot->setRelativeT3D(t);
	checkPose(t);

}
//place the robot in a state but prepared for achieving the target state
void WBState::placeRobotTowards(RobotState *target)
{
	WBState *naux=dynamic_cast<WBState *>(target);
	if(!naux)return;
	Vector3D dif=naux->pose-pose;
	double angle=atan2(dif.y,dif.x);
	angle=getClosestAngle(angle);

	Transformation3D t(pose.x,pose.y,pose.z,Z_AXIS,angle);
	//robot->setRelativeT3D(t);
	checkPose(t);
}
double WBState::getClosestAngle(double ang)
{
double retangle, minimum=2*PI,val;
retangle=ang=Angle::makePositive(ang);

	int num=getNumSectors();
	for(int i=0;i<num;i++)
		{
			if((sectors[i].min<=ang)&&(sectors[i].max>=ang))return ang;
			if((sectors[i].min<ang)&&((val=ang-sectors[i].max)<minimum)){
				retangle=sectors[i].max;
				minimum=val;
				}
			if((sectors[i].max>ang)&&((val=sectors[i].min-ang)<minimum)){
				retangle=sectors[i].min;
				minimum=val;
				}

		}
	return retangle;
}
RobotState *WBState::clone()
{
	return new WBState(*this);
}

WBState::~WBState()
{
}


void WBState::drawGL()
{
	glDisable(GL_LIGHTING);
	glPointSize(2);
	glColor3f(1,1,0);
	glBegin(GL_POINTS);
	glVertex3f(pose.x,pose.y,pose.z);
	glEnd();
	glColor3f(0,0,1);
	int num=getNumSectors();
	for(int i=0;i<num;i++)sectors[i].drawGL();
}

bool WBState::specialize(Transformation3D &t)
{
	//me quedo con el sector valido para la t pasada
	double r,p,y;
	t.orientation.getRPY(r,p,y);
	int num=sectors.size(),i;
	if(num==0)return false;
	for(i=0;i<num;i++)
		if((sectors[i].min<=y)&&(sectors[i].max>=y))
		{
			if(num>1){Sector s=sectors[i];sectors.clear();sectors.push_back(s);}		
			return true;
		}
	return false;
}
double WBState::getSize()
{
	double ret=0;
	for(int i=0;i<(int)sectors.size();i++)
		ret+=sectors[i].max-sectors[i].min;
	return ret;
}
#define SECTOR_RAD 0.15
void WBState::Sector::drawGL()
{
	double val,inc=PI/6.0;
	glPushMatrix();
	
	pose.transformGL();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for(val=min;val<max;val=val+inc)
	{
		//radio 0.05
		glVertex3f(SECTOR_RAD*sin(val),SECTOR_RAD*cos(val),0);
	}
	glVertex3f(SECTOR_RAD*sin(max),SECTOR_RAD*cos(max),0);
	glEnd();

	glPopMatrix();
}