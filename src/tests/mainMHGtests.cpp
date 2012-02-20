#include "../core/vector3d.h"
#include "../core/orientationmatrix.h"
#include <iostream>
using namespace std;
void main()
{
	mr::Vector3D miVector(2,3,4),m;
	m=miVector*4;
	std::cout<<m.x<<" "<<m.values[0]<<std::endl;
	std::cout<<m.y<<" "<<m.values[1]<<std::endl;
	std::cout<<m.z<<" "<<m.values[2]<<std::endl;
	m.normalize();
	std::cout<<m.x<<" "<<m.values[0]<<std::endl;
	std::cout<<m.y<<" "<<m.values[1]<<std::endl;
	std::cout<<m.z<<" "<<m.values[2]<<std::endl;
	std::cout<<"Modulo:"<<m.module()<<endl;
	

	mr::Matrix3x3 unit(1),m1(1,-2,3,4,8,6,9,8,7),m2;
	cout<<unit;
	m2=m1;
	cout<<m1;
	cout<<"determinante="<<m1.getDeterminant()<<endl;
	cout<<m2.inverted();
	cout<<"determinante="<<m2.getDeterminant()<<endl<<endl;
	cout<<m1*m2<<endl;
	
	mr::OrientationMatrix m5,m6(0.78,0.4,0.2);
	cout<<m5<<endl;
	m5=m6;
	cout<<m5<<endl;
	double r,p,y;
	m5.getRotationRPY(r,p,y);
	cout<<0.78<<" "<<r<<std::endl;
	cout<<0.4<<" "<<p<<std::endl;
	cout<<0.2<<" "<<y<<std::endl;
std::cin>>m.y;
}