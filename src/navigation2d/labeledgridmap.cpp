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
 
#include "labeledgridmap.h"
#include "config.h"
#include "data/image.h"
#include "gl/gltools.h"

namespace mr{

IMPLEMENT_MR_OBJECT(LabeledGridMap)

LabeledGridMap::LabeledGridMap():GridMap<unsigned char>(0,0,0){}

LabeledGridMap::LabeledGridMap(double real_width, double real_height, double resolution, unsigned char default_value, double mapX, double mapY)
					:GridMap<unsigned char>(real_width,real_height,resolution,mapX,mapY){

	rgb_colors = new unsigned char*[256];
	
	for (int i=0;i<256;i++) rgb_colors[i]=new unsigned char[3];
	
	
	
	rgb_colors[OBSTACLE][0]=0; rgb_colors[OBSTACLE][1]=0; rgb_colors[OBSTACLE][2]=0;
	rgb_colors[FREE][0]=255; rgb_colors[FREE][1]=255; rgb_colors[FREE][2]=255;
	rgb_colors[UNKNOWN][0]=127; rgb_colors[UNKNOWN][1]=127; rgb_colors[UNKNOWN][2]=127;
	
	fillWithValue(default_value);
	
}

LabeledGridMap::~LabeledGridMap(){
	for (int i=0;i<256;i++) delete [] rgb_colors[i];
	delete [] rgb_colors;
}

Image* LabeledGridMap::convertToImage(int w) const{
	Image *img =new Image(this->width, this->height);
	for (int x=0; x < this->width; x++){
		for (int y=0; y < height; y++){
			unsigned char* color = rgb_colors[ getCellValue(x,y) ];
			img->setPixel(x,height-y-1,color);
		}
	}
	if (w>0 && img->getWidth()!=w) img->scale(w);

	return img;
}


void LabeledGridMap::loadFromImage(Image* img, double real_width, double resolution, double mapX, double mapY){
	this->resolution=resolution;
	this->mapX=mapX;
	this->mapY=mapY;
	this->width=(int)(real_width/resolution);
	if(width != img->getWidth()) img->scale(width);
	this->height=img->getHeight();
	int w,h,c;
	unsigned char*** img_buf = img->getPointerToImageMatrix(w,h,c);
	free();
	allocate(w,h);
	
	for (int i=0; i<w; i++){
		for (int j=0; j<h; j++){
			if (img_buf[i][j][0] == (unsigned char)OBSTACLE) setCellValue(i,height-j-1,OBSTACLE);
			else if (img_buf[i][j][0] == (unsigned char)FREE) setCellValue(i,height-j-1,FREE);
			else setCellValue(i,height-j-1,UNKNOWN);
		}
	}
}

void LabeledGridMap::writeToStream(Stream& os){
	GridMap<unsigned char>::writeToStream(os);
}

void LabeledGridMap::readFromStream(Stream& is){
	GridMap<unsigned char>::readFromStream(is);
}

void LabeledGridMap::drawGL(){


	GridMap<unsigned char>::drawGL();
}

} // end namespace
