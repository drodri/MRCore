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
		
	}
	void MouseMove(int x,int y)
	{
		scene.MouseMove(x,y);
		glutPostRedisplay();
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

int main(int argc,char* argv[])
{
	MyGlutApp myApp("teleop",sim);
	myApp.Run();
	return 0;   
}

int main(int argc, char* argv[])
{
	if(argc<4)
	{
		printUsage();
		return -1;
	}
	string robotname(argv[1]);
	string ip(argv[2]);
	int port=-1;
	if(robotname=="nemo")
		robot=new Nemo;
	else if(robotname=="neo")
		robot=new Neo;
	else if(robotname=="doris")
		robot=new Doris;
	else if(robotname=="ardrone")
	{
		quadrotor=new QuadrotorRobot("ardrone");//robot=0;
	}
	else if(robotname=="kinect")
	{
		kinect=new KinectClient();
	}
	else
	{
		LOG_ERROR("Bad robot name, exiting");
		return -1;
	}
	stringstream ports(argv[3]);
	ports>>port;
	if(port==-1)
	{
		LOG_ERROR("Bad port, exiting");
		return -1;
	}

	//PatrolBot doris connection
	cout<<"Trying to connect robot"<<endl;
	if(robot)
	{
		robot->connectClients(ip,port);
		printInfo();
	}
	if(quadrotor)
	{
		quadrotor->connectClients(ip.c_str(),port);
		printQuadrotorInfo();
	}
	if(kinect)
		kinect->connect(ip.c_str(),port);



	scene.init();
	scene.SetViewPoint(10,20,20);
//Intializing objects
//	scene.addObject();
	if(robot)
		world+=robot->getBase();
	else if(quadrotor)
		world+=quadrotor->getBase();

	scene.addObject(&world);
	scene.addObject(&cloud);
	

	glutTimerFunc(20,OnTimer,0); //set again the timer
		
	glutMainLoop();
	return 0;
}

void OnDraw(void)
{
	Transformation3D t;
	if(quadrotor)
		t=quadrotor->getBase()->getAbsoluteT3D();
	if(robot)
		t=robot->getBase()->getAbsoluteT3D();
	if(robot || quadrotor)
	{
		scene.SetViewCenter(t.position.x,t.position.y,t.position.z);
		double dist,alfa,beta;
		scene.GetViewPoint(dist,alfa,beta);
		double r,p,y;
		t.orientation.getRPY(r,p,y);
		scene.SetViewPoint(dist,y*RAD2DEG+180,beta);
	}

	scene.Draw();
	glutSwapBuffers();
}
static float va=0,vg=0;
static float up=0,forward=0,lateral=0,rot=0;
void OnTimer(int value)
{
	if(robot)
	{
		Odometry odometry;
		LaserData laserData;
		if(robot->getOdometry(odometry))
		{
			//do whatever you want with odometry	
		}
		if(robot->getLaserData(laserData))
		{
			//do whatever you want with laser
		}
		//issue move commands to robot
		robot->move(va,vg);
	}
	if(kinect)
	{
		kinect->getData(cloud);
	}

	glutTimerFunc(20,OnTimer,0); //set again the timer
	glutPostRedisplay();	
}
void OnSpecialKeyboardDown(int key, int x, int y)
{	
	if(quadrotor)
	{
		if(key==GLUT_KEY_UP)
			up+=0.05;
		else if(key==GLUT_KEY_DOWN)
			up-=0.05;
		else if(key==GLUT_KEY_LEFT)
			rot+=0.05;
		else if(key==GLUT_KEY_RIGHT)
			rot-=0.05;

		if(up>1)up=1;if(up<-1)up=-1;
			if(rot>1)rot=1;if(rot<-1)rot=-1;
		quadrotor->move(up,forward,lateral,rot);
	}
}
void OnKeyboardDown(unsigned char key, int x, int y)
{	
	if(robot)
	{
		if(key=='a')
			vg+=0.05;
		else if(key=='d')
			vg-=0.05;
		else if(key=='s')
			va-=0.05;
		else if(key=='w')
			va+=0.05;
		else 
		{
			va=vg=0;
		}
		//	if(va<0)va=0;
	}
	if(quadrotor)
	{
		if(key==' ')
		{
			static bool lan=true;
			if(lan)
			{
				quadrotor->takeOff();
				lan=false;
			}
			else
			{
				quadrotor->land();
				lan=true;
			}
		}
		else
		{
			if(key=='a')
				lateral+=0.05;
			else if(key=='d')
				lateral-=0.05;
			else if(key=='s')
				forward-=0.05;
			else if(key=='w')
				forward+=0.05;
			else
				lateral=forward=up=rot=0;

			
			if(forward>1)forward=1;if(forward<-1)forward=-1;
			if(lateral>1)lateral=1;if(lateral<-1)lateral=-1;
		
			quadrotor->move(up,forward,lateral,rot);
		}
	}

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
	
	scene.MouseButton(x,y,b,down,sKey,ctrlKey);
	glutPostRedisplay();
}
void OnMouseMove(int x,int y)
{
	scene.MouseMove(x,y);
	glutPostRedisplay();
}

void printUsage()
{
	cout<<"-------- Usage -----------------"<<endl;
	cout<<"> teleop {robot} {ip} {port}    "<<endl;
	cout<<"robot can be: nemo doris neo	   "<<endl;
	cout<<"example:    "<<endl;
	cout<<"> teleop neo 127.0.0.1 13000    "<<endl;
}

void printInfo()
{
	cout<<"-------- Teleop -----------------"<<endl;
	cout<<"		An MRCore application   "<<endl;
	cout<<"		w: increase speed		"<<endl;
	cout<<"		s: decrease speed (no backward move)	"<<endl;
	cout<<"		a: rotate left		"<<endl;
	cout<<"		d: rotate right		"<<endl;
	cout<<"		space: stop			"<<endl;
}

void printQuadrotorInfo()
{
	cout<<"-------- Teleop -----------------"<<endl;
	cout<<"		An MRCore application   "<<endl;
	cout<<"		w: increase speed		"<<endl;
	cout<<"		s: decrease speed (no backward move)	"<<endl;
	cout<<"		a: rotate left		"<<endl;
	cout<<"		d: rotate right		"<<endl;
	cout<<"		space: stop			"<<endl;
}