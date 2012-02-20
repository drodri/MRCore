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
#ifndef __MRCORE__LABELEDGRIDMAP_H
#define __MRCORE__LABELEDGRIDMAP_H

#include <iostream>
#include <math.h>
#include <cmath>
#include "gridmap.h"
#include <map>
#include "base/globject.h"

using namespace std;

namespace mr{
	
class Image;
	
	
class LabeledGridMap : public GridMap<unsigned char>, public GLObject {

	///Text stream serializers (files)
	friend ostream& operator << (ostream& os,const LabeledGridMap& gridMap);

	DECLARE_MR_OBJECT(LabeledGridMap)

public:
	enum Labels{ OBSTACLE=0, FREE=255, UNKNOWN=127 };

	LabeledGridMap();
	/**
	 * Constructor of the GridMap class
	 * @param w Real Width (in meters) of the map.
	 * @param h Real Height of the map.
	 * @param resolution. Size (in meters) of on cell in the real world.
	 * @param mapX x-coordinate of the bottom left axis in the real world.
	 * @param mapY y-coordinate of the bottom left axis in the real world.
	 */
	LabeledGridMap(double real_width, double real_height, double resolution, unsigned char default_value = UNKNOWN, double mapX=0, double mapY=0);
	~LabeledGridMap();
	
	/**
	 * Creates a Image from the gridmap keeping the proportions of width width
	 * @param width Width of the image (proportions will be kept. If no value is passed it keeps the map width.
	 */
	Image* convertToImage(int w=0) const;
	
	/**
	 * Builds the map from image file
	 */
	void loadFromImage(Image* img, double real_width, double resolution, double mapX=0, double mapY=0);

	void drawGL();
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);	
private:
	unsigned char** rgb_colors;
	
};


}//end namespace

#endif
