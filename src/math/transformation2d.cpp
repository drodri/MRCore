#include "transformation2d.h"
#include <math.h>
#include <fstream>
#include <string>
#include "gl/gltools.h"
namespace mr{

IMPLEMENT_MR_OBJECT(Transformation2D)

ostream& operator << (ostream& os,const Pose2D& scan)
{
	os<<scan.x<<" "<<scan.y<<" "<<scan.theta<<" ";
	return os;
}


void Transformation2D::writeToStream(Stream& stream)
{
	stream<<x<<y<<theta.getValue();
}
void Transformation2D::readFromStream(Stream& stream)
{
	double th;
	stream>>x>>y>>th;
	theta.setValue(th);
}
void Transformation2D ::transformGL()
{
	glTranslatef(x,y,0);
	glRotatef(theta.getValue()*RAD2DEG,0,0,1);
}

}// namespace
