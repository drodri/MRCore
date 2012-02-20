#include "quadrotordata.h"

namespace mr{

IMPLEMENT_MR_OBJECT(QuadrotorData);
QuadrotorData::QuadrotorData()
{
}
void QuadrotorData::writeToStream(Stream& stream)
{
	Data::writeToStream(stream);
	stream<<battery;
	stream<<altitude;
	stream<<theta;
	stream<<phi;
	stream<<psi;
	stream<<vx;
	stream<<vy;
	
}
void QuadrotorData::readFromStream(Stream& stream)
{
	Data::readFromStream(stream);
	stream>>battery;
	stream>>altitude;
	stream>>theta;
	stream>>phi;
	stream>>psi;
	stream>>vx;
	stream>>vy;
}

};
