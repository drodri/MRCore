/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/

#ifndef __WHEELED_BASE_SIM_H_
#define __WHEELED_BASE_SIM_H_


#include "hw/wheeledbase.h"
#include "base/globject.h"
#include "../world/composedentity.h"
#include "../math/transformation2d.h"

namespace mr
{
class WheeledBaseSim : public WheeledBase, public ComposedEntity
{
	DECLARE_MR_OBJECT(WheeledBaseSim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);

	//Constructor
	WheeledBaseSim(double w=0.35, double l=0.5, double wh_radius=0.1, double wh_width=0.05);

	//Wheeled robot interface realization
	bool getOdometry(Odometry& odom);
	virtual bool move(double speed, double rot);
	virtual bool getPose3D(Pose3D& pose){pose=getAbsoluteT3D();return true;}

	bool computeGroundedLocation(Transformation3D &p,World* w=0);
	//si se quiere que repoduzca el movimiento hay que hacer la operación
	//matemática que la traspasaría a location... de momento es como un reset de los encoders
	virtual void setLocation(const Transformation3D &p)
	{
		setAbsoluteT3D(p);
		odometry.pose=p;
	}
	virtual void simulate(double delta_t);//time inteval in seconds
	virtual void drawGL();
	void getWheelsCenterPoints(Vector3D *w) //4 component vector
		{
			Transformation3D t=getAbsoluteT3D(); //current location
			for(int i=0;i<4;i++)w[i]=t*wheels[i];
		}
	void getSpeed(double &_speed, double &_rotSpeed){
		_speed=speed;
		_rotSpeed=rotSpeed;
	}
	double getWheelRadius(){return wheel_radius;}


	//if the pose is valid, it is stored into t... coution the pose of the wb is modified
    bool dropWheeledBase(Transformation3D &t, World *w=0);
protected:
	Odometry odometry;
	double speed;
	double rotSpeed;
	//generic data for wheels geometry
	double width;
	double large;
	double wheel_radius;//total outer radius
	double wheel_width;
	
	Vector3D wheels[4];



};

}; //end namespace mr

#endif
