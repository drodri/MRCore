/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
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

#ifndef __MRCORE__GRID__H
#define __MRCORE__GRID__H

#include <iostream>
#include <vector>

using namespace std;

namespace mr{

///origin is located left-down, the reference system is normal, with Y pointing up
///The integer indices over the matrix "m" index i follows X axis, and index j follows Y axis
/// the reference frame is located somewhere inside the grid, indicated by the offsets
	//if offset_x=width/2, and offset_y=height/2, the frame will be centered in the grid
///The class is designed to be quickly serialized by memory copying over the "data" array
/// and designed to serve as base class for concrete grids

template <class T>
class Grid
{
public:
	Grid(){data=m=0;width=height=0;}
	virtual ~Grid(){}

	///This is the most common way to initialize a grid
	/// \param r is the resolution (in m) of the grid
	/// this function assumes the grid reference frame is centered in the grid
	void init(int w,int h, double r){init(w,h,r,w/2,h/2);}
	///This function explicitly defines the offset of the reference frame.
	void init(int w,int h, double r, int ox,int oy);
	///Sets all the elments of the grid = value
	void set(T value);

	inline int getWidth()const{return width;}
	inline int getHeight()const{return height;}
	/// \return the size as the number of bytes
	inline unsigned int getSize()const{return width*height*sizeof(T);}
	inline const T* getBuffer()const{return data;}
	
	//TODO:
	//resize methods
	//dump (raw binary serialization)
	
	inline void Grid2Real(int i,int j,float* x,float *y) const
	{
		*x=(-offset_x+i)*resolution;
		*y=(-offset_y+j)*resolution;
	}
	inline void Real2Grid(float x,float y,int* i,int* j) const
	{	
		*i = (int)floor(offset_x + (x / resolution));
		*j = (int)floor(offset_y + (y / resolution));
	}
protected:
	void allocate(int w,int h);
	void free();
	///The size in X
	int width;
	///the size i nY
	int height;
	///The data, as an array
	T* data;
	///The data as a matrix
	T** m;

	///The spatial resolution of the grid
	double resolution;
	///the relative position of the reference frame, in grid integer coordinates 
	int offset_x,offset_y;
	
private:
	///By now, the class is implicitly non-copyable, to avoid overhead
	///If we need to copy, we will implement a custom explicit "copy" method
	Grid(const Grid& g){;}
	void operator=(const Grid& g){;}
};

#include "grid.hpp"

}


#endif
