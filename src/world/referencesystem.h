/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio &Miguel Hernando Gutierrez
 * 
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

#ifndef __MRCORE__REFERENCESYSTEM__H
#define __MRCORE__REFERENCESYSTEM__H

#include <iostream>
#include <vector>
#include "../math/transformation3d.h"

using namespace std;

namespace mr
{
/*!
    \class ReferenceSystem
    ReferenceSystem	->	basic mr object for linking entities positioned
		between them and obtein their relative and absolute positions.
		His aim is to give the functionality for kinematics chains, so
		it stores the dependent reference systems. when modified, all its dependent 
		systems are informed. 
*/
class PositionableEntity;
class ReferenceSystem{

	/**Text output**/
	friend ostream& operator<<(ostream& os, const ReferenceSystem& r);

private:
//attributes

	//Absolute reference: absolute reference for the world
	//if it is a dependent sytems, it is obteined through the 
	//concatenation of the absolute Transformation of its base
	//and the relative transformation
	Transformation3D absolute;

	//Relative reference: relative reference respect absolute reference
	//With the pointer to the base reference system constitutes the 
	//real information of the object
	Transformation3D relative;

	//Need to update. Flag: if true, the value of absolute is probably no
	//correctly computed
	bool needToUpdate;//Necesita actualizarse
	PositionableEntity *owner;
	ReferenceSystem *base;//Father
	vector<ReferenceSystem*> dependentSystem;//Dependent sons

public:
//constructors
	ReferenceSystem(PositionableEntity *_owner=0);
	virtual ~ReferenceSystem(void);

//methods
	void setOwner(PositionableEntity *_owner){owner=_owner;}
	//Modify relative positions

	//Set relative position by vector
	void setRelativePosition(Vector3D rp);

	//Set relative transformation
	inline void setRelativeT3D(const Transformation3D &m){
			relative=m;
			setNeedToUpdate();//Necesita actualizarse
			}

//Modify relative orientations
	//Set relative orientation
	void setRelativeOrientation(double rox, double roy, double roz){
		relative.orientation=OrientationMatrix(rox,roy,roz);
		setNeedToUpdate();
	}
	void setRelativeOrientation(Axis ax, double rot){
		relative.orientation=OrientationMatrix(ax,rot);
		setNeedToUpdate();
	}
//Return absolute and relatives parameters
	//Return absolute transformation
	inline Transformation3D getAbsoluteT3D(){
			doUpdate();//Actualizate
			return absolute;}

	inline ReferenceSystem *getBase(){//Returns the base
			return base;} 
	
	//Return relative transformation
	inline  Transformation3D getRelativeT3D(){
			return relative;}

	//Return absolute position
	inline  Vector3D getAbsolutePosition(){
			doUpdate();//Actualizate
			return absolute.position;}//Retorna la posicion absoluta actualizada
	
	//Return relative position
	inline  Vector3D getRelativePosition(){
			return relative.position;}

//Others
	//Update absolute position if base is outdate
	void doUpdate();

	//Set boolean attribute
	void setNeedToUpdate();

	//Asociate entities
	bool linkTo(ReferenceSystem *rs);
	//unlink reference systems
	void unLink();

	//check if this is one ancentor of rs
	bool isAncestorOf(ReferenceSystem *rs);
	//void dependence(ReferenceSystem * drs);
	bool needsToUpdate(){return needToUpdate;}

	int getNumberOfDependents(){return (int)(dependentSystem.size());}
	ReferenceSystem* getDependent(int i){return dependentSystem[i];}
	PositionableEntity *getOwner(){return owner;}

	void Draw();
	//aplies the OpenGL transformation
	void transformGL();
};

};
#endif  //__MRCORE__REFERENCESYSTEM__H
