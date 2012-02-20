#pragma once

#include "data.h"

namespace mr
{
class QuadrotorData : public Data
{
	DECLARE_MR_OBJECT(QuadrotorData)
	
public:
	QuadrotorData();
//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	
	virtual void drawGL(){};
	
	float battery;///<in percentage 0-100%
	float altitude;///in meters
	float theta;///<angle
	float phi;
	float psi;
	float vx;
	float vy;
	
};

};//namepsace mr
