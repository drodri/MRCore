#include "mrcore.h"
#include <GL/glut.h>
#include <iostream>
#include "glutapp.h"

using namespace mr;
using namespace std;

class MyGlutApp: public GlutApp
{
public:
	MyGlutApp(string name,Simulator& s):GlutApp(name),simulator(s)
	{
		scene.addObject(simulator.getWorld());
	}
	void Draw(void)
	{
		scene.Draw();
	}
	void Timer(float time)
	{
		simulator.getWorld()->simulate(0.020);
	}
	void MouseMove(int x,int y)
	{
		scene.MouseMove(x,y);
		glutPostRedisplay();
	}
	void Key(unsigned char key)
	{
		scene.KeyDown(key);
	}
	void MouseClick(int b,int state, int x,int y)
	{
		bool down=(state==GLUT_DOWN);
		int button;
		if(b==GLUT_LEFT_BUTTON)
			button=MOUSE_LEFT_BUTTON;
		if(b==GLUT_RIGHT_BUTTON)
			button=MOUSE_RIGHT_BUTTON;
			
		int specialKey = glutGetModifiers();
		bool ctrlKey= (specialKey & GLUT_ACTIVE_CTRL)?true:false ;
		bool sKey= specialKey&GLUT_ACTIVE_SHIFT ;
		
		scene.MouseButton(x,y,b,down,sKey,ctrlKey);
		glutPostRedisplay();
	}
private:
	GLScene scene;
	Simulator simulator;	
};

void printUsage();
int main(int argc,char* argv[])
{
	mrcoreInit();
	if(argc!=2)
	{
		printUsage();
		return -1;
	}
	string configFile(argv[1]);

	Simulator sim;
	sim.load(configFile);

	MyGlutApp myApp("MR Simulator",sim);
	myApp.Run();
	return 0;   
}
void printUsage()
{
	cout<<"-------- Usage -----------------"<<endl;
	cout<<"> simulator config.txt    "<<endl;
	cout<<"example:    "<<endl;
	cout<<"> simulator configuration.conf    "<<endl;
}
/*
class Simulator
{
public:
	Simulator()
	{
		initWorld();
		
		robots.push_back(new Neo);
		MobileRobot& neo=*robots[0];
	
		neo.setLocation(Transformation3D(0,-8,0));
		world+=neo.getBase();
		neo.startServers(13000);

	//	robots.push_back(new Doris);
	//	robots.push_back(new Nemo);
	//	robots.push_back(new Neo);

	//	MobileRobot& doris=*robots[0];
	//	MobileRobot& nemo=*robots[1];
	//	MobileRobot& neo=*robots[2];

	//	doris.setLocation(Transformation3D(7,7,0));
	//	world+=doris.getBase();//doris.addToWorld(world);
	//	doris.startServers(12000);

	//	neo.setLocation(Transformation3D(4,1,2));
	//	world+=neo.getBase();
	//	neo.startServers(13000);

	//	nemo.setLocation(Transformation3D(2,3,0,0,0,-M_PI));
	//	world+=nemo.getBase();
	//	nemo.startServers(14000);

	//	quadrotor=new QuadrotorRobot("ardrone");
	//	quadrotor->startServers(15000);
	//	world+=quadrotor->getBase();
	//	quadrotor->setLocation(Transformation3D(5,5, 5));

	//	doris.move(1,1);

		kinect=new KinectSim;
		kinect->setAbsoluteT3D(Transformation3D(2,2,2));
		world+=kinect;
		kinectserver=new KinectServer(kinect,"MyKinect");
		kinectserver->init(20000);
	}
	~Simulator()
	{
		LOG_WARNING("Exiting simulator");
		for(unsigned int i=0;i<robots.size();i++)
			delete robots[i]; 
		delete quadrotor;
		world.destroyContent();
	}


	void initWorld();

	World world;
	vector<MobileRobot* > robots;
	QuadrotorRobot* quadrotor;
	KinectSim* kinect;
	KinectServer* kinectserver;
};

Simulator* simulator;

int main(int argc, char* argv[])
{
	mrcoreInit();
	simulator=new Simulator();
//	DataLogOut datalog;
//	if(!datalog.open("prueba"))
//		return -1;

//	simulator.robots[0]->startLogging(datalog);
	
	//GL Initialization stuff
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("GL");
	glutDisplayFunc(OnDraw);
	glutMotionFunc(OnMouseMove);
	glutMouseFunc(OnMouseClick);
	glutKeyboardFunc(OnKeyboardDown);
	glutTimerFunc(100,OnTimer,0);
	scene.init();

	scene.addObject(&simulator->world);
	
	glutMainLoop();
	return 0;
}

void OnDraw(void)
{  
//	m.Lock();
	scene.Draw();
//	m.Unlock();
	
	glutSwapBuffers();
}
void OnTimer(int value)
{
//	simulator.simulate(0.1);
	simulator->world.simulate(0.020);

	glutTimerFunc(20,OnTimer,0); //set again the timer
	glutPostRedisplay();	
}
void OnKeyboardDown(unsigned char key, int x, int y)
{
	if(key=='s')
	{
		scene.save("glSnapshot");
	//	scene.load("glSnapshot");
	}
//	m.Lock();
	scene.KeyDown(key);
//	m.Unlock();
	

	glutPostRedisplay();	

}
void OnMouseClick(int b,int state, int x,int y)
{
	bool down=(state==GLUT_DOWN);
	int button;
	if(b==GLUT_LEFT_BUTTON)
		button=MOUSE_LEFT_BUTTON;
	if(b==GLUT_RIGHT_BUTTON)
		button=MOUSE_RIGHT_BUTTON;
		
	int specialKey = glutGetModifiers();
	bool ctrlKey= (specialKey & GLUT_ACTIVE_CTRL)?true:false ;
	bool sKey= specialKey&GLUT_ACTIVE_SHIFT ;
	
//		m.Lock();
	scene.MouseButton(x,y,b,down,sKey,ctrlKey);
//	m.Unlock();
	
	glutPostRedisplay();
}
void OnMouseMove(int x,int y)
{
//		m.Lock();
	scene.MouseMove(x,y);
//	m.Unlock();
	
	
	glutPostRedisplay();
}

void Simulator::initWorld()
{
	Face suelo(Transformation3D(0,0,0),-5,-10,5,10);
	FaceSetPart *building=new FaceSetPart; 
	building->addFace(suelo);

	for(float x=-5;x<=5;x+=1)
		for(float y=-5;y<=5;y+=1)
		{	
			PrismaticPart *mypart=new PrismaticPart;
			vector<Vector2D> list;
			list.push_back(Vector2D(0,0));
			list.push_back(Vector2D(0,.2));
			list.push_back(Vector2D(.2,.2));
			list.push_back(Vector2D(.2,0));
			mypart->setPolygonalBase(list);
			mypart->setRelativePosition(Vector3D(x+0.5*(((int)y)%2),y,0));
			mypart->setRelativeOrientation(0,0,PI/2);
			mypart->setHeight(1);
			world+=mypart;
		}
	world+=building;

	//Intializing test environment Faces included in a FacePart
//	Face suelo(Transformation3D(0,0,0),0,-10,10,10);
//	Face tablon_fino1(Transformation3D(8,3,2,X_AXIS,-0.53),0,0,0.2,3.95);
//	Face tablon_fino2(Transformation3D(8.5,3,2,X_AXIS,-0.53),0,0,0.2,3.95);
//	Face tablon_grueso(Transformation3D(2,3,2,X_AXIS,-0.53),0,0,1,3.95);
//	Face plataforma(Transformation3D(2,0,2),0,0,8,3);
//	Face paredfondo1(Transformation3D(0,0,0,Y_AXIS,PI/2),-4,-10,0,10);
//	Face paredfondo2;
//
//	paredfondo2.setBase(Transformation3D(0,0,0,X_AXIS,-PI/2));
//	paredfondo2.addVertex(0,-4);
//	paredfondo2.addVertex(10,-4);
//	paredfondo2.addVertex(10,0);
//	paredfondo2.addVertex(6,0);
//	paredfondo2.addVertex(6,-1.5);
//	paredfondo2.addVertex(4,-1.5);
//	paredfondo2.addVertex(4,0);
//	paredfondo2.addVertex(0,0);
//
//	FaceSetPart *building=new FaceSetPart; 
//	building->addFace(suelo);
//	building->addFace(tablon_fino1);
//	building->addFace(tablon_fino2);
//	building->addFace(tablon_grueso);
//	building->addFace(plataforma);
//	building->addFace(paredfondo1);
//	building->addFace(paredfondo2);
//	
//	world+=building;
//
//	FaceSetPart *paux2=new FaceSetPart(*building);
//	paux2->setRelativeOrientation(0,0,PI);
//	paux2->setRelativePosition(Vector3D(20,0,0));
//	world+=paux2;
	
//it works!!!
}

*/
