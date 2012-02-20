/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Miguel Hernando & Diego Rodríguez-Losada & Alberto Valero
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
#include "vector3d.h"

#include <math.h>

namespace mr{

ostream& operator<<(ostream& os, const Vector3D& p)
{
	os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

Vector3D &Vector3D::normalize()
{
	double mod;
	mod = module();
	if(mod>(double)0.00001)
	{
		x/=mod;
		y/=mod;
		z/=mod;
	}
	return *this;
}
Vector3D Vector3D::getUnitaryVector() const
{
	Vector3D ret(*this);
	return ret.normalize();
}
} //mr
