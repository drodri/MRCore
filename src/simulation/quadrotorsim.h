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

#ifndef __QUADROTOR_SIM_H_
#define __QUADROTOR_SIM_H_


#include "hw/quadrotor.h"
#include "base/globject.h"
#include "../world/composedentity.h"
#include "../math/transformation2d.h"

namespace mr
{
class QuadrotorSim : public Quadrotor, public ComposedEntity
{
	DECLARE_MR_OBJECT(QuadrotorSim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Constructor
	QuadrotorSim(double radius=0.25);
	virtual ~QuadrotorSim(){}

	//Wheeled robot interface realization
	virtual bool move(double up, double forward,double lateral,double rot);
	virtual bool getData(QuadrotorData& data){data=quadrotorData;return true;}
	virtual bool takeOff(){motorsOn=true;upSpeed=0.1;return true;}
	virtual bool land(){motorsOn=false;return true;}
	
	//si se quiere que repoduzca el movimiento hay que hacer la operación
	//matemática que la traspasaría a location... de momento es como un reset de los encoders
	virtual void setLocation(const Transformation3D &p){setRelativeT3D(p);}
	virtual void simulate(double delta_t);//time inteval in seconds
	virtual void drawGL();

protected:
	double radius;//total outer radius
	double upSpeed,forwardSpeed,lateralSpeed,rotSpeed;
	bool motorsOn;
	QuadrotorData quadrotorData;

};

}; //end namespace mr

#endif
