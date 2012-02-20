#include "segment3d.h"
#include "gl/gltools.h"

namespace mr{

IMPLEMENT_MR_OBJECT(Segment3D)


void Segment3D::drawGL()
{
	GLTools::Color(YELLOW);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glVertex3f(p1.x,p1.y,p1.z);
	glVertex3f(p2.x,p2.y,p2.z);
	glEnable(GL_LIGHTING);
	glEnd();
}

}


