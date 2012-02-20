/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Mrgroup team
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

#ifndef __MRCORE__MATERIAL_H
#define __MRCORE__MATERIAL_H




#include "../base/object.h"
namespace mr{

//currently a very simplified version. In future it will be posible to
//define colors and materials in an easy way
class Material
{
struct Color{
	double r;
	double g;
	double b;
	} color;
public:
	Material(){color.r=0.5; color.g=0.5; color.b=0.5;}
	void setColor(double red, double green, double blue){
		color.r=red;
		color.g=green;
		color.b=blue;
	}
	void getColor(double &red, double &green, double &blue){
		red=color.r;green=color.g;blue=color.b;
	}
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);
	void loadMaterial();
};


}


#endif
