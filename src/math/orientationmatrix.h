/**********************************************************************
 *
 * This code is part of the MRcore project
 * Authors:  Miguel Hernando, Diego Rodríguez-Losada, Paloma de la Puente,
 *			 Alberto Valero, Luis Pedraza
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
#ifndef __MRCORE__ORIENTATIONMATRIX_H
#define __MRCORE__ORIENTATIONMATRIX_H

#include <iostream>

#include "matrix3x3.h"

namespace mr
{
/*!
    \class OrientationMatrix
    \brief OrientationMatrix - a basic mathematical class for 3x3 matrix.
	It is any 3x3 matrix not only an orientation matrix, therefore, 
	operations like inversion are made through a general formula. Another
	consequence is that the matrix elements are public.
*/
enum Axis {X_AXIS, Y_AXIS, Z_AXIS};
class OrientationMatrix:public Matrix3x3{

private:
	inline OrientationMatrix(const Matrix3x3 &m):Matrix3x3(m){}
public:
//constructors
	inline OrientationMatrix():Matrix3x3(1){}
	OrientationMatrix(double roll, double pitch, double yaw);
	OrientationMatrix(Axis axis,double ang);
	OrientationMatrix(Vector3D vu, Vector3D vv);
//operators
	inline const double * operator [](int i){return mat[i];}
//only a rotation matrix could =,*,*= with another rotation matrix 
	inline OrientationMatrix & operator= (const OrientationMatrix &m){
		matrix=m.matrix;
		return *this;
	}
	inline OrientationMatrix operator * (const OrientationMatrix &m) const{
		OrientationMatrix ret(*this);
		ret.Matrix3x3::operator*=(m);
		return ret;
	}
	inline OrientationMatrix & operator *= (const OrientationMatrix &m){
		Matrix3x3::operator *=(m);
		return *this;
	}
	inline Vector3D operator *(const Vector3D &v){return Matrix3x3::operator *(v);}

//methods
	//Roll pitch yaw convention (eq to rotation trx, ry, rz fixed axis)
	OrientationMatrix & setRPY(double roll, double pitch, double yaw);
	void getRPY(double& roll, double& pitch, double& yaw);

	inline double getDeterminant(){return 1;} 
	inline OrientationMatrix inverted(){
		return Matrix3x3::transposed();
	}
	inline OrientationMatrix transposed(){
		return Matrix3x3::transposed();
	}	
	inline Vector3D getVectorU(){
		return Vector3D(mat[0][0],mat[1][0],mat[2][0]);
	}
	inline Vector3D getVectorV(){
		return Vector3D(mat[0][1],mat[1][1],mat[2][1]);
	}
	inline Vector3D getVectorW(){
		return Vector3D(mat[0][2],mat[1][2],mat[2][2]);
	}

//friend operators


	
//friend methods

};







}
#endif  //__MRCORE__ORIENTATIONMATRIX_H
