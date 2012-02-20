// PruebaMRcore.cpp : Defines the entry point for the console application.
//

#include "mrcore.h"
/*#include "gl/glscene.h"
#include "core/laserdata.h"
#include "core/math/plane.h"
#include "core/math/segment3d.h"
#include "core/math/vector3d.h"
#include "core/math/orientationmatrix.h"
#include "core/math/transformation3d.h"*/
#include <GL/glut.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace mr;
using namespace std;

void OnDraw(void);
void OnTimer(int value); //esta funcion sera llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); //cuando se pulse una tecla	
void OnMouseMove(int x,int y);
void OnMouseClick(int button,int state, int x,int y);


//The scene global object
GLScene scene;
LaserData laser;
vector<Plane> planes;
vector<Segment3D> segments;
Segment3D segment;
Face face;
Vector2D facedot;
//test dof transformation3d
//a kinematic chain of two elements
Transformation3D tone,ttwo;

int main(int argc, char* argv[])
{
	//GL Initialization stuff
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("GL");
	glutDisplayFunc(OnDraw);
	glutMotionFunc(OnMouseMove);
	glutMouseFunc(OnMouseClick);
	glutKeyboardFunc(OnKeyboardDown);

	scene.init();
//Intializing objects
	vector<double> datos;
	for(int i=0;i<181;i++)
		datos.push_back(8);
	laser.setData(datos,-90,1);

	/*planes.push_back(Plane(Vector3D(5,0,2),Vector3D(1,0,0)));
	planes.push_back(Plane(Vector3D(-5,0,2),Vector3D(1,0,0)));
	planes.push_back(Plane(Vector3D(0,7,2),Vector3D(0,1,0)));
	planes.push_back(Plane(Vector3D(0,-7,2),Vector3D(0,1,0)));
	planes.push_back(Plane(Vector3D(0,0,0),Vector3D(0,0,1)));
	planes.push_back(Plane(Vector3D(0,3,10),Vector3D(0,1,1)));
	planes.push_back(Plane(Vector3D(0,-3,10),Vector3D(0,-1,1)));
*/

	for(int i=0;i<10;i++)
		segments.push_back(Segment3D(Vector3D(i,-10,2),Vector3D(i,10,0)));
	
//Adding objects to scene
	scene.addObject(&laser);
	for(int i=0;i<(int)(planes.size());i++)
		scene.addObject(&planes[i]);
	for(int i=0;i<(int)(segments.size());i++)
		scene.addObject(&segments[i]);

	face.addVertex(0,0);
	face.addVertex(8,0);
	face.addVertex(8,8);
	face.addVertex(5,8);
	face.addVertex(5,3);
	face.addVertex(3,3);
	face.addVertex(3,8);
	face.addVertex(0,8);
	//face.setOrigin(Transformation3D(0,0,0,0.5,0.5,0.5));

	tone=Transformation3D(0,2,0);
	ttwo=Transformation3D(0,0,3);

	segment=Segment3D(Vector3D(4,4,-1), Vector3D(5,2,6));
	scene.addObject(&segment);
	glutMainLoop();

	return 0;
}

void OnDraw(void)
{  Transformation3D dot;
	dot.position=Vector3D(facedot.x,facedot.y);
	/*if(face.isInteriorPoint(facedot))laser.drawGLMode=2;
	else laser.drawGLMode=0;*/
	if(Interactions::faceSegment(face,segment,dot.position))laser.drawGLMode=2;
	else laser.drawGLMode=0;
	scene.Draw();
	tone.drawGL();
	(tone*ttwo).drawGL();
	//face.setOrigin(tone*ttwo);
	face.drawGL();

	dot.drawGL();
	glutSwapBuffers();
}
void OnTimer(int value)
{
	//glutTimerFunc(1000,OnTimer,0); //set again the timer
	glutPostRedisplay();	
}
void OnKeyboardDown(unsigned char key, int x, int y)
{
	static double angle=0;
	if(key=='0')
		laser.drawGLMode=0;
	if(key=='1')
		laser.drawGLMode=1;
	if(key=='2')
		laser.drawGLMode=2;
	if(key=='3')
		laser.drawGLMode=3;
	if(key=='+')
		tone.orientation=OrientationMatrix(X_AXIS,angle=angle+0.02);
	if(key=='-')
		tone.orientation=OrientationMatrix(X_AXIS,angle=angle-0.02);
	if(key=='d')facedot.x+=0.1;
	if(key=='a')facedot.x-=0.1;
	if(key=='w')facedot.y+=0.1;
	if(key=='x')facedot.y-=0.1;
	
	segment.p1.x=facedot.x;
	segment.p1.y=facedot.y;
	
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
