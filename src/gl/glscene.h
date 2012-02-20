#pragma once

#include "base/globject.h"
#include <string>
#include <vector>



using namespace std;

namespace mr
{
//enums to eliminate dependency of glut
enum {MOUSE_LEFT_BUTTON, MOUSE_MIDDLE_BUTTON, MOUSE_RIGHT_BUTTON};
enum {KEY_UP, KEY_DOWN, KEY_LEFT,KEY_RIGHT};

class GLScene
{
public:
	GLScene();
	virtual ~GLScene();
	//Serializers
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);
	//file helpers, use .glscene extension
	bool save(string filename);//without extension
	bool load(string filename);//without extension

//Initizalization
	void init();//enable lights

	void setShowGrid(bool yes=true){showGrid=yes;}
	void setShowFrame(bool yes=true){showFrame=yes;}

	
//Object Managers
	void setObjects(vector<GLObject*> v);
	void clearObjects();
	void addObject(GLObject* object);
	void removeObject(GLObject* object);

//Event managers
	void Draw();
	void KeyDown(unsigned char key);
	void SpecialKeyDown(unsigned char key);
	void MouseMove(int x,int y);//The mouse coordinates
	void MouseButton(int x,int y,int button,bool down,bool shiftKey, bool ctrlKey);
	
	//info
	void GetViewPoint(double& d, double& rot,double& elv){d=dist;rot=alfa;elv=beta;}
	void SetViewPoint(float distance, float rotation, float elevation){dist=distance;alfa=rotation; beta=elevation;}
	void SetViewCenter(float x,float y,float z){center_x=x;center_y=y;center_z=z;}

	
protected:
	vector<GLObject*>  object;
	vector<GLObject*>  loadedObjects;//objects owned by the scene

//visualization	
	double center_x,center_y,center_z;
	double dist,beta,alfa;
	double x_previo,y_previo;
	double vx,vy,vz;//viewpoint
	bool showGrid,showFrame;
	bool controlKey;
	bool shiftKey;
	bool leftButton;
	bool rightButton;
	bool midButton;
};
}; //namespace mr
