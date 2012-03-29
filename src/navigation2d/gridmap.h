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

#ifndef MRCORE_GRIDMAP_H_
#define MRCORE_GRIDMAP_H_


#include <iostream>
#include <vector>
#include "math/vector2d.h"
#include "math/transformation2d.h"
#include <cmath>
#include <set>
#include "data/image.h"
#include "base/globject.h"
#include "gl/gltools.h"


using namespace std;

namespace mr {

template <class T>
class GridMap {

	///Text stream serializers (files)
	template<class T1>
	friend ostream& operator << (ostream& os,const GridMap<T1>& gridMap);

public: 

	struct RayPoint{
		RayPoint(){;}
		RayPoint(int px, int py, double wx, double wy,T v){pixelX=px;pixelY=py;x=wx;y=wy;}
		int pixelX, pixelY;
		double x,y;
	};

	typedef vector<RayPoint> Ray;

public:

	GridMap(){texture=0; width=height=0; resolution=0;buffer=NULL;grid=NULL; updated=false;}
	/**
	 * Constructor of the GridMap class
	 * @param real_width Real Width (in meters) of the map.
	 * @param real_height Real Height of the map.
	 * @param resolution. Size (in meters) of on cell in the real world.
	 * @param mapX x-coordinate of the bottom left axis in the real world.
	 * @param mapY y-coordinate of the bottom left axis in the real world.
	 */
	GridMap(double real_width, double real_height, double resolution, double mapX=0, double mapY=0);
	virtual ~GridMap(){free(); glDeleteTextures( 1, &texture );}
	
	virtual Image* convertToImage(int w=0) const = 0;

	/**
	 * It resizes the grid to the passed width
	 * @param grid_width New width of the grid. Height is changed to keep proportions
	 */
	void scale(int grid_width);

	T* getPointerToBuffer(int& buffer_size){return buffer;}
	T* getBufferClone(int& buffer_size){
		buffer_size = sizeof(T)*width*sizeof(T)*height;
		T* buf = new T(buffer_size);
		memcpy(buf,buffer,buffer_size);
		return buf;
	}

	inline T getCellValue(int x, int y) const {return *grid[x][y]; };
	inline void setCellValue(int x, int y, T value) {*grid[x][y]=value; updated=true;}
	/**
	 * Returns true if the value of the pixel if it is within the map boundaires
	 */
	inline bool getSafeCellValue(int x, int y, T& value) const {
		if (x<width && x>=0 && y <height && y>=0){
			value = getCellValue(x,y); return true;
		}else{
			cerr <<"CellValue "<< x <<" "<< y <<" out of map boudaries"<< endl;
			return false;
		}
	}

	inline bool setSafeCellValue(int x, int y, T value){
		if (x<width && x>=0 && y <height && y>=0){
			setCellValue(x,y,value); return true;
		}else{
			cerr <<"CellValue "<< x <<" "<< y <<" out of map boudaries"<< endl;
			return false;
		}
	}

	inline double getRealWidth(){return resolution*width;}

	inline double getRealHeight(){return resolution*height;}

	inline double getResolution(){return resolution;}

	inline int getWidth(){return width;}
	inline int getHeight(){return height;}
	inline double getMapX(){return mapX;}
	inline double getMapY(){return mapY;}

	inline bool GridToWorld(int gridX, int gridY, double &worldX, double &worldY){
		if (gridX < width && gridY < height){
			worldX = mapX + resolution*gridX;
			worldY = mapY + resolution*gridY;
			return true;
		}else{
			return false;
		}
	}

	inline bool GridToWorld(int gridX, int gridY, Vector2D& point){
		double x,y;
		bool rt=GridToWorld(gridX,gridY,x,y);
		point.x=x;
		point.y=y;
		return rt;
	}

	inline bool WorldToGrid(double worldX, double worldY, int& gridX, int& gridY){
		gridX = (int)((worldX-mapX)/resolution);
		gridY = (int)((worldY-mapY)/resolution);
		if (gridX < width && gridY < height) return true;
		else return false;
	}

	inline bool WorldToGrid(Vector2D point, int& gridX, int& gridY){
		gridX = (int)((point.x-mapX)/resolution);
		gridY = (int)((point.y-mapY)/resolution);
		if (gridX < width && gridY < height) return true;
		else return false;
	}

	inline void fillWithValue(T value) //!< Fill the map with a value
	{
		updated=true;
		for (int i=0;i<width;i++){
			for (int j=0;j<height;j++){
				*grid[i][j]=value;
			}
		}
	}
	
	/**
	 * Returns true when there is a pixel with value "value" in the traced ray. The ray has a maxlength = maxdistnace
	 * @param x The x world init coordinate of the ray
	 * @param y The y world init coordinate of the ray 
	 * @param angle The angle (wrt the x world axe) of the ray
	 * @param value Value of the pixel we are comaring with.
	 * @param maxdistance Length of the ray
	 * @param rayPoint. If the pixel is found in rayPoint the information of the pixel is stored
	 * @return true if a pixel of value value is found
	 */
	bool isThereCellValueInRay(double x, double y, double angle, T value, double maxdistance, RayPoint& rayPoint);
	
	inline bool isThereCellValueInRay(Vector2D point, double angle, T value, double maxdistance, RayPoint& rayPoint){
		return isThereCellValueInRay(point.x, point.y, angle, value, maxdistance, rayPoint);
	}
	
	
	inline bool isThereCellValueInRay(Pose2D point, double angle, T value, double maxdistance, RayPoint& rayPoint){
		return isThereCellValueInRay(point.x, point.y, point.theta+angle, value, maxdistance, rayPoint);
	}
	
	
	/**
	 * Returns true when there is a pixel with value "value" in the ray between two points.
	 * @param point1 The starting point of the ray
	 * @param Vector2D The ending point of the ray
	 * @param value Value of the pixel we are comaring with.
	 * @param rayPoint. If the pixel is found in rayPoint the information of the pixel is stored
	 * @return true if a pixel of value value is found
	 */
	inline bool isThereCellValueBetweenPoints(Vector2D point1, Vector2D Vector2D, T value, RayPoint& rayPoint)
	{
		mr::Vector2D directionVector = Vector2D - point1;
		return isThereCellValueInRay(point1.x, point1.y, directionVector.argument(), value, point1.distance2(Vector2D), rayPoint);
	}
	
	inline bool isThereCellValueBetweenPoints(double x1, double y1, double x2, double y2, T value, RayPoint& rayPoint){
		return isThereCellValueBetweenPoints(Vector2D(x1,y1), Vector2D(x2,y2), value, rayPoint);
	}
	
	/**
	 * Returns true when there is a pixel with value in "valueSet" in the traced ray. The ray has a maxlength = maxdistnace
	 * @param x The x world init coordinate of the ray
	 * @param y The y world init coordinate of the ray 
	 * @param angle The angle (wrt the x world axe) of the ray
	 * @param valueSet Set of values of the pixel we are comaring with.
	 * @param maxdistance Length of the ray
	 * @param rayPoint. If the pixel is found in rayPoint the information of the pixel is stored
	 * @return true if a pixel of value value is found
	 */
	bool isThereValueSetInRay(double x, double y, double angle, set<T> valueSet, double maxdistance, RayPoint& rayPoint);

	inline bool isThereCellValueSetInRay(Vector2D point, double angle, set<T> valueSet, double maxdistance, RayPoint& rayPoint){
		return isThereCellValueInRay(point.x, point.y, angle, valueSet, maxdistance, rayPoint);
	}
	
	
	inline bool isThereCellValueSetInRay(Pose2D point, double angle, set<T> valueSet, double maxdistance, RayPoint& rayPoint){
		return isThereCellValueInRay(point.x, point.y, point.theta+angle, valueSet, maxdistance, rayPoint);
	}


	/****
	 * Returns a vector of RayPoints (pixels, world coords, and grid value).
	 * @param x The x world init coordinate of the ray
	 * @param y The y world init coordinate of the ray 
	 * @param angle The angle (wrt the x world axe) of the ray
	 * @param distance Length of the ray
	 * @return The vector of RayPoint composing the ray.
	 */
	Ray getRay(double x, double y, double angle, double distance);

	inline Ray getRay(Vector2D point, double angle, double distance){
		return getRay(point.x, point.y, angle, distance);
	}


	inline Ray getRay(Pose2D point, double angle, double distance){
		return getRay(point.x, point.y, point.theta + angle, distance);
	}

	Ray getRay(Vector2D pointi, Vector2D pointf);

inline void writeToStream(Stream& os){
	int buffer_size=width*sizeof(T)*height*sizeof(T);
	os<<width<<height<<resolution<<mapX<<mapY;
	for (int i=0;i<buffer_size;i++)
		os<<buffer[i];
}

inline void readFromStream(Stream& is){
	is >> width >> height >> resolution >> mapX >> mapY;
	int buffer_size=width*sizeof(T)*height*sizeof(T);
	this->free();
	allocate(width,height);
	for (int i=0;i<buffer_size;i++){
		unsigned char reading;
		is >> reading;
		buffer[i]=reading;
	}
	updated=true;
}

void drawGL();



protected:
	T* getPointerToCell(int x, int y){return grid[x][y];}
	int width, height; //!<the gridmap width and height (rows, cols)
	int buffer_size;
	double mapX, mapY; //!<The map coordinates of (in the world) the bottom-left corner
	T* buffer; //!<The buffer in a one dimension array
	void allocate(int w, int h); //!< Allocate the memory for the map
	void free(); //!< Free the memeory space
	double resolution; //!< the gridmap resolution = meters/pixel

	bool updated;
	GLuint texture;



private:
	T*** grid; //!<The gridmap

};


#include "gridmap.hpp"

} // end namespace

#endif /* GRIDMAP_H_ */
