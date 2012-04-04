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

#include "grid.h"

template <class T>
void Grid<T>::allocate(int w,int h)
{
	free();
	data=new T[w*h];
	m=new T*[w];
	for(int i=0;i<w;i++)
		m[i]=data+h*i;

	width=w;
	height=h;
}
template <class T>
void Grid<T>::free()
{
	delete data;
	delete m;
	width=height=0;
}
	
template <class T>
void Grid<T>::init(int w,int h, double r, int ox,int oy)
{
	allocate(w,h);
	resolution=r;
	offset_x=ox;
	offset_y=oy;
}
template <class T>
void Grid<T>::set(T value)
{
	for(int i=0;i<width*height;i++)
		data[i]=value;
}
