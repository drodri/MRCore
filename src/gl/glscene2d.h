#pragma once

#include "base/globject.h"
#include <string>
#include <vector>

#define UP_ARROW    119
#define LEFT_ARROW  97
#define DOWN_ARROW  115
#define RIGHT_ARROW 100


using namespace std;

namespace mr{
	
class GLScene2D
{
public:
	GLScene2D();
	virtual ~GLScene2D();
	//Serializers
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);
	//file helpers, use .glscene extension
	bool save(string filename);//without extension
	bool load(string filename);//without extension

//Initizalization
	void init();//enable lights
	
//Object Managers
	void setObjects(vector<GLObject*> v);
	void clearObjects();
	void addObject(GLObject* object);
	void removeObject(GLObject* object);
	void setMap(GLObject* map, double initX, double initY, double width, double height);
	
//Event managers
	void Draw();
	void KeyDown(unsigned char key);
	//void SpecialKeyDown(unsigned char key);
	void MouseMove(int x,int y);//The mouse coordinates
	void MouseButton(int x,int y,int button,bool down,bool shiftKey, bool ctrlKey);
	
	//info
	void GetViewPoint(double&xi, double& yi,double& xf, double& yf){xi=initX;yi=initY;xf=finalX;yf=finalY;}
	void SetViewPoint(double xi, double yi,double xf, double yf){initX=xi;initY=yi;finalX=xf;finalY=yf;}

	
protected:
	vector<GLObject*>  object;
	vector<GLObject*>  loadedObjects;//objects owned by the scene

//visualization	
	double initX,initY,finalX,finalY;
	bool showGrid,showFrame;
	bool controlKey;
	bool shiftKey;
	bool leftButton;
	bool rightButton;
	bool midButton;
};

}; //namespace mr
