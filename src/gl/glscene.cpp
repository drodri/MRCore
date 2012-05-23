#include "gltools.h"
#include "glscene.h"
#include "../math/mrmath.h"
#include "../base/streamfile.h"
#include "../world/world.h"
#include <math.h>
#include <fstream>
#include <iostream>
namespace mr
{
bool GLScene::save(string filename)//without extension 
{
	StreamFile str(filename+".glscene",false);
	if(!str.good())return false;
	writeToStream(str); 
	return true;
}
bool GLScene::load(string filename)//without extension
{
	StreamFile str(filename+".glscene",true);
	if(!str.good())return false;
	readFromStream(str);
	return true;
}
void GLScene::writeToStream(Stream& stream)
{
	int n=(int)object.size();
	stream<<n;
	for(int i=0;i<n;i++)
		stream.write(object[i]);
}
void GLScene::readFromStream(Stream& stream)
{
	int n;
	stream>>n;
	for(int i=0;i<n;i++)
	{
		Object* obj=stream.read();
		GLObject* globj=dynamic_cast<GLObject*>(obj);
		if(globj)
			loadedObjects.push_back(globj);
		else
			delete obj;
	}
}
GLScene::GLScene()
{
	dist=100;
	center_x=center_y=0;
	center_z=1;
	beta=15;
	alfa=30;
	//by default, the view size is 800*600
	ox=0;
	oy=0;
	h=600;
	w=800;
	showGrid=false;
	showFrame=true;
	BackgroundColor(0.2f,0.2f,0.2f);
}
void GLScene::BackgroundColor(float  r, float  g, float  b)
{
	back_r=r;
	back_b=b;
	back_g=g;
}
GLScene::~GLScene()
{
	for(unsigned int i=0;i<loadedObjects.size();i++)
	{
		//World* w=dynamic_cast<World*>(loadedObjects[i]);
		//if(w)
		//{
		//	delete w;
		//}
		//else
		//{
			delete loadedObjects[i];
		//}
	}
}
void GLScene::KeyDown(unsigned char key)
{
	if(key=='g')
		showGrid=!showGrid;
	if(key=='f')
		showFrame=!showFrame;
	if(key=='t')
		Face::enableAlfa=!Face::enableAlfa;
		
}
void GLScene::SpecialKeyDown(unsigned char key)
{
	if(key==KEY_UP)
	{
		center_x-=0.075*cos(alfa*DEG2RAD);
		center_y-=0.075*sin(alfa*DEG2RAD);
	}
	if(key==KEY_DOWN)
	{
		center_x+=0.075*cos(alfa*DEG2RAD);
		center_y+=0.075*sin(alfa*DEG2RAD);
	}
	if(key==KEY_LEFT)
	{
		center_x-=0.075*cos((alfa+90)*DEG2RAD);
		center_y-=0.075*sin((alfa+90)*DEG2RAD);
	}
	if(key==KEY_RIGHT)
	{
		center_x+=0.075*cos((alfa+90)*DEG2RAD);
		center_y+=0.075*sin((alfa+90)*DEG2RAD);
	}
}
void GLScene::init()
{	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, ((GLdouble)w)/((GLdouble)h), 1.1, 150);

}
void GLScene::Draw() 
{
	glViewport((GLint)ox,(GLint)oy,(GLint)w,(GLint)h);
	//Borrado de la pantalla	
	glClearColor(back_r,back_g,back_b,1);//fondo naranja
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Defines the perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 40.0, ((GLdouble)w)/((GLdouble)h), 1.1, 150);
	
	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	vz=center_z+dist*(float)sin(beta*DEG2RAD);
	vx=center_x+dist*(float)(cos(beta*DEG2RAD)*cos(alfa*DEG2RAD));
	vy=center_y+dist*(float)(cos(beta*DEG2RAD)*sin(alfa*DEG2RAD));
	
	gluLookAt(vx,vy,vz,center_x,center_y,center_z,0,0,1);
	
	if(showGrid)
		GLTools::DrawGrid(50,2);
	if(showFrame)
		GLTools::DrawFrame();
	
	for(unsigned int i=0;i<object.size();i++)
	{
		object[i]->drawGL();
	}
	for(unsigned int i=0;i<loadedObjects.size();i++)
	{
		loadedObjects[i]->drawGL();
	}
}
void GLScene::MouseButton(int x,int y,int button,bool down,bool sKey, bool ctrlKey)
{
	x_previo=x;
	y_previo=y;
	
	if(down)
	{
		controlKey=ctrlKey;
		shiftKey=sKey;
	}
	else
		controlKey=shiftKey=false;
			
	if(button==MOUSE_LEFT_BUTTON)
		leftButton=down;
	else if(button==MOUSE_RIGHT_BUTTON)
		rightButton=down;
	else if(button==MOUSE_MIDDLE_BUTTON)
		midButton=down;
}
void GLScene::MouseMove(int x,int y)
{
	if(controlKey)
	{
		if(leftButton)
		{		
			center_y-=(float)(cos(alfa*DEG2RAD)*(float)(x_previo-x)/10.0f
							+sin(alfa*DEG2RAD)*(float)(y_previo-y)/10.0f);
			center_x-=(float)(-sin(alfa*DEG2RAD)*(float)(x_previo-x)/10.0f
							+cos(alfa*DEG2RAD)*(float)(y_previo-y)/10.0f);
			x_previo=x;
			y_previo=y;	
		}
		if(rightButton)
		{	
			center_z+=(float)(y_previo-y)/10.0f;
			x_previo=x;
			y_previo=y;	
		}
	}
	else
	{
		if(leftButton)
		{
			beta-=y_previo-y;
			if(beta>90.0f)beta=90.0f;
			if(beta<0)beta=0;
			alfa+=x_previo-x;
			x_previo=x;
			y_previo=y;	
		}
		if(rightButton)
		{
			dist+=(y_previo-y)/10.0F;
			if(dist<0)dist=0;
			if(dist>300.0f)dist=300.0f;
			x_previo=x;
			y_previo=y;	
		}	
	}
}
void GLScene:: clearObjects()
{
	object.clear();
}
void GLScene::setObjects(vector<GLObject*> v)
{
	object=v;
}
void GLScene::addObject(GLObject *o)
{
	object.push_back(o);
}
void GLScene::removeObject(GLObject* obj)
{
	for(unsigned int i=0;i<object.size();i++)
	{
		if(object[i]==obj)
		{
		//	delete obj;
			object.erase(object.begin()+i);
			break;
		}
	}
}
void GLScene::setViewSize(int x, int y, int width, int height)
{
	ox=x;
	oy=y;
	if(width>0)w=width;
	if(height>0)h=height;
}
/*
void GLScene::Draw(Vector3D p)
{
	glBegin(GL_POINTS);
		glVertex3f(p.x,p.y,p.z);
	glEnd();
}
void GLScene::Draw(const Trans3D& t)
{
	Apply(t);
	DrawFrame();
}
*/

/*void GLScene::Draw(const PointCloud3D& t)
{
	glDisable(GL_LIGHTING);
	glColor3ub(255,255,0);
	glPointSize(1);
	int i,j;
	glBegin(GL_POINTS);
	for(i=0;i<t.data.size();i++)
		for(j=0;j<t.data[i].size();j++)
		{
			Vector3D p=t.data[i][j];
			glVertex3f(p.x,p.y,p.z);
		}
	glEnd();
}*/

/*void GLScene::Draw(const PointCloud3D& t)
{
	glDisable(GL_LIGHTING);
	glColor3ub(255,255,0);
	glPointSize(1);
	int i,j;
	glBegin(GL_POINTS);
	
	ColumnVector scan_init_pose(6);
	
	if (t.global_poses.size() > 0)
	{
		scan_init_pose(1) = t.global_poses[0][0];
		scan_init_pose(2) = t.global_poses[0][1];
		scan_init_pose(3) = t.global_poses[0][2];
		scan_init_pose(4) = t.global_poses[0][3];
		scan_init_pose(5) = t.global_poses[0][4];
		scan_init_pose(6) = t.global_poses[0][5];
	}
		
	for(i=0;i<t.data.size();i++)
	{
		ColumnVector scan_pose(6);
		scan_pose(1) = t.global_poses[i][0];
		scan_pose(2) = t.global_poses[i][1];
		scan_pose(3) = t.global_poses[i][2];
		scan_pose(4) = t.global_poses[i][3];
		scan_pose(5) = t.global_poses[i][4];
		scan_pose(6) = t.global_poses[i][5];
			
		for(j=0;j<t.data[i].size();j++)
		{
			Vector3D p=t.data[i][j];
			
			ColumnVector v(6);
			ColumnVector inc_odom(6);
			v << Newmat3DOps::Inv(scan_init_pose);
			inc_odom << Newmat3DOps::Comp(v,scan_pose);
			//points referred to local reference frame located at pose from where the 3D scan was initially taken
			Trans3D aux_t;
			aux_t.DirectTrans(inc_odom(1),inc_odom(2),inc_odom(3),inc_odom(4),inc_odom(5),inc_odom(6));
			Vector3D l_point = aux_t.Trans(p);
			
			if (fabs(l_point.z) <= 0.2)
				glColor3ub(255,0,0);	//red
			else
				glColor3ub(255,255,0); //yellow
				
			if (l_point.z < 1 && l_point.z > 0.2)
				glColor3ub(0,255,0); //green
			if (l_point.z < -0.2 )
				glColor3ub(0,0,255);	//blue
				
			
			glVertex3f(l_point.x,l_point.y,l_point.z);
		}
	}
	glEnd();
}*/

/*
void GLScene::Draw(const PointCloud3D& pcloud)
{
	glDisable(GL_LIGHTING);
	glColor3ub(255,255,0);
	glPointSize(1);
	int i,j;
	
	//glPushMatrix();
	glBegin(GL_POINTS);
	
	ColumnVector scan_init_pose(6);
		
	for(i=0;i<pcloud.data.size();i++)
	{
		ColumnVector scan_pose(6);
		double x = pcloud.global_poses[i][0];
		double y = pcloud.global_poses[i][1];
		double z = pcloud.global_poses[i][2];
		double roll = pcloud.global_poses[i][3];
		double pitch = pcloud.global_poses[i][4];
		double yaw = pcloud.global_poses[i][5];			
		
		Trans3D global_t;
		global_t.DirectTrans(x,y,z,roll,pitch,yaw);
		
		
		//Apply(global_t);
		
		for(j=0;j<pcloud.data[i].size();j++)
		{
			Vector3D p=pcloud.data[i][j];
			
			Vector3D global_point = global_t.Trans(p);
			
			glVertex3f(global_point.x,global_point.y,global_point.z);
			//glVertex3f(p.x,p.y,p.z);
		}
		
		
		
		
	}
	glEnd();
	//glPopMatrix();
}


*/
};//Namespace mr
