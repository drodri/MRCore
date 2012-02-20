/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Alberto Valero
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
 
#include "occupancygridmap255.h"
#include "config.h"
#include "data/image.h"
#include "gl/gltools.h"

namespace mr{
	
IMPLEMENT_MR_OBJECT(OccupancyGridMap255)
OccupancyGridMap255::OccupancyGridMap255():GridMap<unsigned 
char>(0,0,0){}
OccupancyGridMap255::OccupancyGridMap255(double real_width, double real_height, double resolution, unsigned char default_value, double mapX, double mapY)
					:GridMap<unsigned char>(real_width,real_height,resolution,mapX,mapY){
	
	fillWithValue(default_value);
	
}

OccupancyGridMap255::~OccupancyGridMap255(){
}

Image* OccupancyGridMap255::convertToImage(int w) const{
	Image *img =new Image(this->width, this->height);
	for (int x=0; x < this->width; x++){
		for (int y=0; y < height; y++){
			unsigned char gray_level= GridMap<unsigned char>::getCellValue(x,y);
			unsigned char color[3]={ gray_level, gray_level, gray_level };
			img->setPixel(x,height-y-1,color);
		}
	}
	if (w>0 && img->getWidth()!=w) img->scale(w);

	return img;
}

void OccupancyGridMap255::writeToStream(Stream& os){
	GridMap<unsigned char>::writeToStream(os);
}

void OccupancyGridMap255::readFromStream(Stream& is){
	GridMap<unsigned char>::readFromStream(is);
}

void OccupancyGridMap255::drawGL(){

	GridMap<unsigned char>::drawGL();
}


}//end namespace
