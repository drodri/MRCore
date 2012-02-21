#include "mrcore.h"

#include <GL/glut.h>
#include <iostream>
#include "glutapp.h"

using namespace mr;
using namespace std;

class MyGlutApp: public GlutApp
{
public:
	MyGlutApp(string name,MobileRobot* r):GlutApp(name),robot(r)
	{
		world+=robot;
		scene.addObject(&world);
		va=vg=0;
	}
	void Draw(void)
	{
		scene.Draw();
	}
	void Timer(float time)
	{
		robot->move(va,vg);
		Odometry odom;
		LaserData laserData;

		robot->getOdometry(odom);
		robot->getLaserData(laserData);
	}
	void Key(unsigned char key)
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
	float vg,va;
	GLScene scene;
	World world;
	MobileRobot* robot;
	
};

int main(int argc,char* argv[])
{
	MobileRobot* robot=new Neo();
	robot->connectClients("127.0.0.1",13000);
	MyGlutApp myApp("teleop",robot);
	myApp.Run();
	return 0;   
}