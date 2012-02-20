#include "nemolasersensor3dsim.h"
#include <iostream>
#include "gl/gltools.h"
#include "../world/tcp.h"


namespace mr
{
IMPLEMENT_MR_OBJECT(NemoLaserSensor3DSim)

NemoLaserSensor3DSim::NemoLaserSensor3DSim()
{

	//creation of the powercube
	arm=new PowerCube70Sim;
	//arm->setRelativePosition(Vector3D(0.1,0,0.285));
	(*this)+=arm; 
	//creation of the LMS200 laser scanner
	laser=new LMS200Sim;
	laser->setColor(0,1,0);
	laser->setRelativePosition(Vector3D(0,0,0.063));
	laser->LinkTo(arm->getTcp());//plug in the laser at the end of the arm

}
void NemoLaserSensor3DSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
}
void NemoLaserSensor3DSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
}
void NemoLaserSensor3DSim::drawGL()
{
	//it si possible to make the drawing independent of the geometric model
	//so we overwrite the composedentity drawing function with this one
	ComposedEntity::drawGL();
	LaserSensor3DSim::drawGL();
	return;
}
void NemoLaserSensor3DSim::updateSensorData()
{
	float pan,tilt;
	arm->GetPos(pan,tilt);
	MRTime timer;
	timer.precistic();
		vector<LaserData> auxL;
		vector<Transformation3D> auxT;
		Transformation3D r=(getAbsoluteT3D()).inverted();
		LaserData las;
		for(double alfa=-M_PI/4; alfa<M_PI/4;alfa+=0.05)
		{
			arm->setPos(pan,tilt+alfa);
			Transformation3D t=laser->getAbsoluteT3D();
			laser->updateSensorData();
			laser->getData(las);
			auxL.push_back(las);
			
			auxT.push_back(r*t);	
		}
		m.Lock();
		data.timeStamp();
		data.set(auxL,auxT);
		m.Unlock();
		
	double tiempo=timer.precistoc();
//	std::cout<<tiempo<<std::endl;
	arm->setPos(pan,tilt);
	laser->updateSensorData();
}

}; //Namespace mr
